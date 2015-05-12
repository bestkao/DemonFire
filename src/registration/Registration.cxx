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

	FirstPassMetricType::Pointer metric1 = FirstPassMetricType::New();
	SecondPassMetricType::Pointer metric2 = SecondPassMetricType::New();
	AmoebaOptimizerType::Pointer amoebaoptimizer = AmoebaOptimizerType::New();
	GradientOptimizerType::Pointer gradientoptimizer = GradientOptimizerType::New();
    RegistrationType::Pointer   registration  = RegistrationType::New();
    
    amoebaoptimizer->OptimizeWithRestartsOn();

    gradientoptimizer->SetLearningRate(4);
	gradientoptimizer->SetMinimumStepLength(0.001);
	gradientoptimizer->SetRelaxationFactor(0.5);
	gradientoptimizer->SetNumberOfIterations(200);
	gradientoptimizer->AddObserver( itk::IterationEvent(), CommandIterationUpdate::New() );

    
	// gradient is faster but more memory intensive
	registration->SetOptimizer(gradientoptimizer);
	registration->SetMetric(metric1);

  
	registration->SetFixedImage(fixed);
	registration->SetMovingImage(moving);
	
	// initialize transform
	TransformType::Pointer identityTransform = TransformType::New();
	identityTransform->SetIdentity();
	
    registration->SetMovingInitialTransform(identityTransform);
	registration->SetFixedInitialTransform(identityTransform);
	
    RegistrationType::ShrinkFactorsArrayType shrinkFactorsPerLevel;
    shrinkFactorsPerLevel.SetSize( 1 );
    shrinkFactorsPerLevel[0] = 64;
    shrinkFactorsPerLevel[1] = 32;
    shrinkFactorsPerLevel[2] = 16;
    RegistrationType::SmoothingSigmasArrayType smoothingSigmasPerLevel;
    smoothingSigmasPerLevel.SetSize( 3 );
    smoothingSigmasPerLevel[0] = 0;
    smoothingSigmasPerLevel[1] = 0;
    smoothingSigmasPerLevel[2] = 0;
    registration->SetNumberOfLevels ( 1 );

    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    registration->SetShrinkFactorsPerLevel( shrinkFactorsPerLevel );
    registration->SetSmoothingSigmasPerLevel( smoothingSigmasPerLevel );
    
    registration->SetMetricSamplingStrategy( RegistrationType::RANDOM );
    registration->SetMetricSamplingPercentage(.20);
    
       // do the actual registration
    registration->Update();
    
    /*
    std::cout << "First pass registration finished with following results:" << std::endl;
    std::cout << gradientoptimizer->GetCurrentIteration() << " = ";
    std::cout << gradientoptimizer->GetValue() << " : ";
    std::cout << gradientoptimizer->GetCurrentPosition() << std::endl;
    
    TransformType::ConstPointer transform = registration->GetTransform();
    
    // second round using mutual information
    registration  = RegistrationType::New();
    registration->SetFixedImage(fixed);
	registration->SetMovingImage(moving);
    registration->SetOptimizer(amoebaoptimizer);
	registration->SetMetric(metric2);
    registration->SetNumberOfLevels ( 1 );
    registration->SetMetricSamplingStrategy( RegistrationType::RANDOM );
    registration->SetMetricSamplingPercentage(.20);
    
    registration->SetMovingInitialTransform(transform);
	registration->SetFixedInitialTransform(identityTransform);
    registration->Update();
    
    std::cout << "Registration finished with following results:" << std::endl;
    amoebaoptimizer->Print(std::cout); */
	
	return registration->GetTransform();
    }
}