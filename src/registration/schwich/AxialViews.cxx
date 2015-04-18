//
// This example demonstrates how to read a series of dicom images
// and how to scroll with the mousewheel or the up/down keys
// through all slices
//

#include <algorithm>
// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
// headers needed for this example
#include <vtkImageViewer2.h>
#include <vtkDICOMImageReader.h>
#include <vtkInteractorStyleImage.h>
#include <vtkActor2D.h>
#include <vtkTextProperty.h>
#include <vtkTextMapper.h>
// needed to easily convert int to std::string
#include <sstream>

// itk-vtk
#include <itkImage.h>
#include <itkImageFileReader.h>

#include <itkImageToVTKImageFilter.h>

#include "vtkVersion.h"
#include "vtkImageViewer.h"
#include "vtkImageMapper3D.h"
#include "vtkImageActor.h"
#include "itkRGBPixel.h"

#include <vtkImageMapper.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkCommand.h>

// reading DICOM in itk
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"
#include "CineView.cxx"

// needed for basic image registration
#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"


// helper class to format slice status message
class StatusMessage {
public:
   static std::string Format(int slice, int maxSlice) {
      std::stringstream tmp;
      tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
      return tmp.str();
   }
};

class vtkAnimation : public vtkCommand
{
public:
    static vtkAnimation *New()
    {
        vtkAnimation *cb = new vtkAnimation;
        cb->slice = 0;
        return cb;
    }
    
    virtual void Execute(vtkObject *caller, unsigned long eventId,
                         void * vtkNotUsed(callData))
    {
        if (vtkCommand::TimerEvent == eventId)
        {
            ++this->slice;
        }
        
        if(this->slice > this->max){
            this->slice = min;
        }
        mapper->SetSliceNumber(this->slice);
        vtkRenderWindowInteractor *iren = vtkRenderWindowInteractor::SafeDownCast(caller);
        iren->GetRenderWindow()->Render();
    }
    
    virtual void SetMapper(vtkImageSliceMapper *mapper){
        this->mapper = mapper;
        this->min = mapper->GetSliceNumberMinValue();
        this->max = mapper->GetSliceNumberMaxValue();
        this->slice = this->min;
    }
    
private:
    int slice, max, min;
public:
    vtkImageSliceMapper* mapper;
};

int main(int argc, char* argv[])
{
    // Constants and type definitions
    //typedef signed short PixelType;
    typedef float PixelType;
    const unsigned int Dimension = 3;
    typedef itk::Image<PixelType, Dimension> ImageType;
    typedef itk::ImageSeriesReader< ImageType > ReaderType;
    typedef itk::ImageToVTKImageFilter<ImageType>       ConnectorType;
    typedef itk::GDCMImageIO ImageIOType;
    typedef itk::GDCMSeriesFileNames NamesGeneratorType;
    typedef std::vector< std::string > FileNameList;
    
    // need to define the component types needed for registration
    // types of input images
	typedef itk::Image<PixelType, Dimension> FixedImageType; // fixed image is the base image
	typedef itk::Image<PixelType, Dimension> MovingImageType;
	// transform maps the fixed image space into the moving image space
	// type of transform
	typedef itk::TranslationTransform<double, Dimension> TransformType;

	// optimizer explores the parameter space of the transform in search of optimal values of the metric
	// type of optimizer
	typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType; 

	// metric compares how well the two images match each other
	// type of metric
	typedef itk::MeanSquaresImageToImageMetricv4<FixedImageType, MovingImageType> MetricType;

	// the registration method is responsible for interconnecting all of the components
	// type of registration method
	typedef itk::ImageRegistrationMethodv4<FixedImageType, MovingImageType, TransformType> RegistrationType;
    
   /*// Verify input arguments
   if ( argc != 2 )
   {
      std::cout << "Usage: " << argv[0]
      << " FolderName" << std::endl;
      return EXIT_FAILURE;
   }*/
   
	// need two arguments, 1 for each folder
	if (argc != 3) {
		std::cout << "Usage: " << argv[0]
		<< " FixedFolderName" 
		<< " MovingFolderName" << std::endl; 
		return EXIT_FAILURE;
	}

    // pull path from command line args for now
	std::string folder = argv[1];
	std::string folder2 = argv[2];
    
    ReaderType::Pointer reader = ReaderType::New();
    ConnectorType::Pointer connector = ConnectorType::New();
    
    // need another reader for Moving Image
    ReaderType::Pointer reader2 = ReaderType::New();
    
    // read data in as DICOM image using GDCMImageIO class
    ImageIOType::Pointer dicomIO = ImageIOType::New();
    reader->SetImageIO( dicomIO );
    
    // same for second image series
    ImageIOType::Pointer dicomIO2 = ImageIOType::New();
    reader2->SetImageIO(dicomIO2);
    
    // generate file names for DICOM image
    NamesGeneratorType::Pointer nameGenerator = NamesGeneratorType::New();
    nameGenerator->SetDirectory(folder);
    nameGenerator->SetUseSeriesDetails( true );
    //nameGenerator->AddSeriesRestriction("0008|0021" );    // filter by additional requirements
    const FileNameList & seriesUID = nameGenerator->GetSeriesUIDs();
    
    // same for second image series
    NamesGeneratorType::Pointer nameGenerator2 = NamesGeneratorType::New();
    nameGenerator2->SetDirectory(folder2);
    nameGenerator2->SetUseSeriesDetails(true);
    const FileNameList &seriesUID2 = nameGenerator2->GetSeriesUIDs();
    
    // print out series information to help with debugging
    FileNameList::const_iterator seriesItr = seriesUID.begin();
    FileNameList::const_iterator seriesEnd = seriesUID.end();
    while( seriesItr != seriesEnd )
    {
        std::cout << "Found series " << seriesItr->c_str() << " containing "
            << nameGenerator->GetFileNames( seriesItr->c_str() ).size() << " files" << std::endl;
        ++seriesItr;
    }
    
    // for now, visualize the first series found in the input folder
    std::string seriesIdentifier = seriesUID.begin()->c_str();
    FileNameList fileNames = nameGenerator->GetFileNames( seriesIdentifier );
    
    // same for second series
    std::string seriesIdentifier2 = seriesUID2.begin()->c_str();
    FileNameList fileNames2 = nameGenerator2->GetFileNames(seriesIdentifier2);
    
	reader->SetFileNames(fileNames);
	reader2->SetFileNames(fileNames2);
	
    
    // instantiate all of the components needed for registration and hook them up 
    MetricType::Pointer metric = MetricType::New();
	OptimizerType::Pointer optimizer = OptimizerType::New();
	RegistrationType::Pointer registration = RegistrationType::New();

	// each component is now connected to the instance of the registration method
	registration->SetMetric(metric);
	registration->SetOptimizer(optimizer);

	// metric needs interpolators are evaluate the intensities of the fixed and moving images
	// at non-grid posititions
	// types of the interpolators
	typedef itk::LinearInterpolateImageFunction<FixedImageType, double> FixedLinearInterpolatorType;
	typedef itk::LinearInterpolateImageFunction<MovingImageType, double> MovingLinearInterpolatorType;

	// then, create the interpolators and pass them to the metric
	FixedLinearInterpolatorType::Pointer fixedInterpolator =
		FixedLinearInterpolatorType::New();
	MovingLinearInterpolatorType::Pointer movingInterpolator =
		MovingLinearInterpolatorType::New();

	metric->SetFixedInterpolator(fixedInterpolator);
	metric->SetMovingInterpolator(movingInterpolator);
	
	// make the image registration aquire inputs from the outputs of the readers
	try
    {
        reader->Update();
        reader2->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }
	registration->SetFixedImage(reader->GetOutput());
	registration->SetMovingImage(reader2->GetOutput());
	
	TransformType::Pointer movingInitialTransform = TransformType::New();
	TransformType::ParametersType initialParameters(movingInitialTransform->GetNumberOfParameters());
	initialParameters[0] = 0.0; // initial offset in mm along X
	initialParameters[1] = 0.0; // initial offset in mm along y
	initialParameters[2] = 0.0; // initial offset in mm along z
	
	movingInitialTransform->SetParameters(initialParameters);
	registration->SetMovingInitialTransform(movingInitialTransform);
	
	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	
	registration->SetFixedInitialTransform(identityTransform);
	
	// optimizer settings
	optimizer->SetLearningRate(4);
	optimizer->SetMinimumStepLength(0.001);
	optimizer->SetRelaxationFactor(0.5);
	optimizer->SetNumberOfIterations(200);
	
	try {
		registration->Update();
		
	} catch (itk::ExceptionObject &err) {
		std::cerr << "ExceptionObject caught!" << std::endl;
		std::cerr << err << std::endl;
		return EXIT_FAILURE;
	}
	
	/*TransformType::ConstPointer transform = registration->GetTransform();
	TransformType::ParametersType finalParameters = transform->GetParameters();
	const double TranslationAlongX = finalParameters[0];
	const double TranslationAlongY = finalParameters[1];
	const double TranslationAlongZ = finalParameters[2];*/
	
	typedef itk::CompositeTransform<double, Dimension> CompositeTransformType;
	CompositeTransformType::Pointer outputCompositeTransform = CompositeTransformType::New();
	outputCompositeTransform->AddTransform(movingInitialTransform);
	outputCompositeTransform->AddTransform(registration->GetModifiableTransform());

	typedef itk::ResampleImageFilter<MovingImageType, FixedImageType> ResampleFilterType;
	ResampleFilterType::Pointer resampler = ResampleFilterType::New();
	resampler->SetInput(reader2->GetOutput());
	resampler->SetTransform(outputCompositeTransform);
	
	FixedImageType::Pointer fixedImage = reader->GetOutput();
	resampler->SetSize(fixedImage->GetLargestPossibleRegion().GetSize());
	resampler->SetOutputOrigin(fixedImage->GetOrigin());
	resampler->SetOutputSpacing(fixedImage->GetSpacing());
	resampler->SetOutputDirection(fixedImage->GetDirection());
	resampler->SetDefaultPixelValue(100);
	
	try {
		connector->SetInput(resampler->GetOutput());
		connector->Update();
		
	} catch (itk::ExceptionObject &ex) {
		std::cerr << ex << std::endl;
		return EXIT_FAILURE;
	}
    
    /*// read image from list of filenames as an itk Image object
    // convert itk image to a vtkImageData object with connector
    reader->SetFileNames( fileNames );
    try
    {
        reader->Update();
        connector->SetInput(reader->GetOutput());
        connector->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }*/
    
    cout << "We've got a " << connector->GetOutput()->GetDataDimension() << "d vtk image" << endl;

   // Visualize using VTK
    // slice of 3d image
    
    // Setup render window containing vtk visualization (no other GUI elements for now)
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(900, 600);
    
    // Setup render window interactor (defines how user interacts with visualization)
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    vtkSmartPointer<vtkInteractorStyleImage> interactorStyle =
    vtkSmartPointer<vtkInteractorStyleImage>::New();
    renderWindowInteractor->SetInteractorStyle(interactorStyle);
    
    renderWindowInteractor->SetRenderWindow(renderWindow);
    
    
    // Define viewport ranges
    double xmins[3] = {0,.35,.7};
    double xmaxs[3] = {0.3,.65,1.};
    for(unsigned i = 0; i < 3; i++)
    {
        cineView *cr = new cineView(connector->GetOutput(), i);
        
        renderWindow->AddRenderer(cr->getRenderer());
        cr->getRenderer()->SetViewport(xmins[i],0.0,xmaxs[i],1.0);
        cr->addAnimationObserver(renderWindowInteractor);
    }
    
    // Render and start interaction
    renderWindow->Render();
    renderWindow->SetWindowName("Axial Views");
    renderWindowInteractor->Start();

   return EXIT_SUCCESS;
}
