#include "DemonFire.h"

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

namespace fire {
    // this is a function that takes in fixed and moving images
    // and performs registration on them. Returns a transform for the moving image
    TransformType::ConstPointer doRegistration(ImageType::Pointer fixed, ImageType::Pointer moving){

	MetricType::Pointer metric = MetricType::New();
	OptimizerType::Pointer optimizer = OptimizerType::New();
    TransformType::Pointer      transform     = TransformType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();
	
	registration->SetOptimizer(optimizer);
	registration->SetMetric(metric);
  
	registration->SetFixedImage(fixed);
	registration->SetMovingImage(moving);
	
	// initialize transform
	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
    registration->SetMovingInitialTransform(identityTransform);
	registration->SetFixedInitialTransform(identityTransform);
	
	// optimizer settings
    optimizer->SetLearningRate(4);
	optimizer->SetMinimumStepLength(0.001);
	optimizer->SetRelaxationFactor(0.5);
	optimizer->SetNumberOfIterations(200);
	optimizer->AddObserver( itk::IterationEvent(), CommandIterationUpdate::New() );
	
    const unsigned int numberOfLevels = 1;
    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 3 );
    shrinkFactorsPerLevel[0] = 4;
    shrinkFactorsPerLevel[1] = 2;
    shrinkFactorsPerLevel[2] = 1;
    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 3 );
    smoothingSigmasPerLevel[0] = 0;
    smoothingSigmasPerLevel[1] = 0;
    smoothingSigmasPerLevel[2] = 0;
    registration->SetNumberOfLevels ( numberOfLevels );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    
    registration->SetMetricSamplingStrategy( RegistrationType::RANDOM );
    registration->SetMetricSamplingPercentage(.20);
    
    // do the actual registration
    registration->Update();
    
    std::cout << "Registration finished with following results:" << std::endl;
    std::cout << optimizer->GetCurrentIteration() << " = ";
    std::cout << optimizer->GetValue() << " : ";
    std::cout << optimizer->GetCurrentPosition() << std::endl;
	
	return registration->GetTransform();
    }
}