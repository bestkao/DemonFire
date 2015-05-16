// Generate a file graph.txt for a series of moving and fixed images, which contains mean sqaures metric information

#include <fstream>
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
#include <itkTranslationTransform.h>
#include <itkShrinkImageFilter.h>

#include "itkCommand.h"

// Software Guide : BeginCodeSnippet
#include "itkMeanSquaresImageToImageMetric.h"
#include "itkTranslationTransform.h"
#include "itkNearestNeighborInterpolateImageFunction.h"
// Software Guide : EndCodeSnippet


// helper class to format slice status message
class StatusMessage {
public:
   static std::string Format(int slice, int maxSlice) {
      std::stringstream tmp;
      tmp << "Slice Number  " << slice + 1 << "/" << maxSlice + 1;
      return tmp.str();
   }
};

class CommandIterationUpdate : public itk::Command
{
public:
    typedef CommandIterationUpdate Self;
    typedef itk::Command Superclass;
    typedef itk::SmartPointer<Self> Pointer;
    typedef itk::RegularStepGradientDescentOptimizerv4<double> OptimizerType;
    typedef const OptimizerType * OptimizerPointer;
    itkNewMacro( Self );
protected:
    CommandIterationUpdate() {};
public:
    void Execute(itk::Object *caller, const itk::EventObject & event)
    {
        Execute( (const itk::Object *)caller, event);
    }

    void Execute(const itk::Object * object, const itk::EventObject & event)
    {
        OptimizerPointer optimizer =
        static_cast< OptimizerPointer >( object );
        if( ! itk::IterationEvent().CheckEvent( &event ) )
        {
            return;
        }
        std::cout << optimizer->GetCurrentIteration() << " = ";
        std::cout << optimizer->GetValue() << " : ";
        std::cout << optimizer->GetCurrentPosition() << std::endl;
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
    const unsigned int Dimension = 3; // Currently working on 3 dimension
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
    typedef itk::ShrinkImageFilter<ImageType, ImageType> ShrinkFilter;

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

    // same for 2nd series
    FileNameList::const_iterator series2Itr = seriesUID2.begin();
    FileNameList::const_iterator series2End = seriesUID2.end();
    while (series2Itr != series2End) {
		std::cout << "Found series " << series2Itr->c_str() << " containing "
		<< nameGenerator2->GetFileNames(series2Itr->c_str()).size() << " files" << std::endl;
		++series2Itr;
	}

	std::cout << " Found both series." << std::endl;

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

	std::cout << "Created all registration components." << std::endl;

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

    std::cout << "Read both image series into memory." << std::endl;

    ShrinkFilter::Pointer shrink = ShrinkFilter::New();
    shrink->SetInput(reader->GetOutput());
    shrink->SetShrinkFactors(16);
    ShrinkFilter::Pointer shrink2 = ShrinkFilter::New();
    shrink2->SetInput(reader2->GetOutput());
    shrink2->SetShrinkFactors(16);

    shrink->Update();
    shrink2->Update();
    std::cout << "Shrunk both images." << std::endl;


// The type of the Metric is instantiated and one is constructed.  In this case
// we decided to use the same image type for both the fixed and the moving
// images.
//

  typedef itk::MeanSquaresImageToImageMetric<
                            ImageType, ImageType >  MetricType2;
  MetricType2::Pointer metric2 = MetricType2::New();
// Software Guide : EndCodeSnippet
// Software Guide : BeginLatex
//
// We also instantiate the transform and interpolator types, and create objects
// of each class.

  typedef itk::TranslationTransform< double, Dimension >  TransformType;
  TransformType::Pointer transform = TransformType::New();
  typedef itk::NearestNeighborInterpolateImageFunction<
                                 ImageType, double >  InterpolatorType;
  InterpolatorType::Pointer interpolator = InterpolatorType::New();


  transform->SetIdentity();
  ImageType::ConstPointer fixedImage  = shrink->GetOutput();
  ImageType::ConstPointer movingImage = shrink2->GetOutput();

// The classes required by the metric are connected to it. This includes the
// fixed and moving images, the interpolator and the  transform.

  metric2->SetTransform( transform );
  metric2->SetInterpolator( interpolator );
  metric2->SetFixedImage(  fixedImage  );
  metric2->SetMovingImage( movingImage );
// Software Guide : EndCodeSnippet
  metric2->SetFixedImageRegion(  fixedImage->GetBufferedRegion()  );
  try
    {
    metric2->Initialize();
    }
  catch( itk::ExceptionObject & excep )
    {
    std::cerr << "Exception catched !" << std::endl;
    std::cerr << excep << std::endl;
    return EXIT_FAILURE;
    }

// Finally we select a region of the parametric space to explore. In this case
// we are using a translation transform in 2D, so we simply select translations
// from a negative position to a positive position, in both $x$ and $y$. For
// each one of those positions we invoke the GetValue() method of the Metric.
//
// Software Guide : EndLatex
// Software Guide : BeginCodeSnippet
std::ofstream myfile;
  myfile.open ("graph.txt");

  MetricType2::TransformParametersType displacement( Dimension );
  const int rangex = 50;
  const int rangey = 50;
  for( int dx = -rangex; dx <= rangex; dx++ )
    {
    for( int dy = -rangey; dy <= rangey; dy++ )
      {
      displacement[0] = dx;
      displacement[1] = dy;
      const double value = metric2->GetValue( displacement );
      std::cout << dx << "   "  << dy << "   " << value << std::endl;
      myfile << dx << "   "  << dy << "   " << value << std::endl;
      }
    }
myfile.close();


   return EXIT_SUCCESS;
}
