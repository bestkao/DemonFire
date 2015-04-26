#include <sys/types.h>
#include <sys/stat.h>
// unix headers
#include <unistd.h>
#include <dirent.h>
#include <ftw.h>

#include <algorithm>
// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkObjectFactory.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkActor.h>
// headers needed for some examples, clean up if unused later
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

#include <vtkVersion.h>
#include <vtkImageViewer.h>
#include <vtkImageMapper3D.h>
#include <vtkImageActor.h>
#include <itkRGBPixel.h>

#include <vtkImageMapper.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkCommand.h>

// reading/writing DICOM in itk
#include <itkGDCMImageIO.h>
#include <itkGDCMSeriesFileNames.h>
#include <itkNumericSeriesFileNames.h>
#include <itkImageSeriesReader.h>
#include <itkImageSeriesWriter.h>

// needed for basic image registration
#include <itkImageRegistrationMethodv4.h>
#include <itkTranslationTransform.h>
#include <itkMeanSquaresImageToImageMetricv4.h>
#include <itkRegularStepGradientDescentOptimizerv4.h>

#include "itkCommand.h"

typedef short PixelType;
const unsigned int Dimension = 3;
typedef itk::Image<PixelType, Dimension> ImageType;
typedef itk::Image<PixelType, Dimension-1> OutputImageType;
typedef itk::ImageSeriesReader< ImageType > ReaderType;
typedef itk::ImageSeriesWriter<ImageType, OutputImageType >  WriterType;
typedef itk::ImageToVTKImageFilter<ImageType> ConnectorType;
typedef itk::TranslationTransform< double, Dimension > TransformType;
typedef itk::GDCMImageIO ImageIOType;
typedef itk::GDCMSeriesFileNames NamesGeneratorType;
typedef itk::NumericSeriesFileNames NumericNamesGeneratorType;
typedef std::vector< std::string > FileNameList;

class Registration{
public:
    static TransformType::ConstPointer doRegistration(ImageType::Pointer, ImageType::Pointer);
};

class Segmentation{
public:
    static ImageType::Pointer doSegmentation(ImageType::Pointer);
};

class ImageProcessing{
public:
    static ImageType::Pointer doPreProcessing(ImageType::Pointer);
    static ImageType::Pointer smoothImage(ImageType::Pointer);
    static ImageType::Pointer sharpenImage(ImageType::Pointer);
    static ImageType::Pointer applyMask(ImageType::Pointer, ImageType::Pointer);
    static ImageType::Pointer closeImage(ImageType::Pointer);
    static ImageType::Pointer shrinkImage(int factor);
};

class Utility{
public:
    static ImageType::Pointer loadDICOM(char*);
    static int saveDICOM(ImageType::Pointer, char*);
    static std::vector<ImageType::Pointer> loadDICOMs(char* folder);
};
