#include "itkImageRegistrationMethodv4.h"
#include "itkTranslationTransform.h"
#include "itkMeanSquaresImageToImageMetricv4.h"
#include "itkRegularStepGradientDescentOptimizerv4.h"

// dimension of image
const unsigned int Dimension = 3;

// type of pixel
typedef float PixelType;

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

// now that we have defined all of the types of the components
// we instantiate them using New()

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

// aquire registration inputs from the outputs of the readers



