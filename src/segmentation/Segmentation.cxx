#include "DemonFire.h"
#include "itkConnectedThresholdImageFilter.h"

//Currently this code only works with the SegmentationImageSeries because the seed is hardcoded

namespace fire {
  ImageType::Pointer doSegmentation (ImageType::Pointer input){
    //the region growing filter type
    typedef itk::ConnectedThresholdImageFilter<ImageType, ImageType > ConnectedFilterType;
    
    //the region growing filter
    ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
    
    connectedThreshold->SetInput(input);
    
    //the uppper and lower thresholds and the seeds are hardcoded for now
    const PixelType lowerThreshold = 5;
    const PixelType upperThreshold = 100;
    connectedThreshold->SetLower(lowerThreshold);
    connectedThreshold->SetUpper(upperThreshold);
    
    ImageType::IndexType index; //seed
    index[0] = 217;
    index[1] = 224;
    index[2] = 1;
    connectedThreshold->SetSeed(index);
    connectedThreshold->SetReplaceValue(255);
    
    try {
      connectedThreshold->Update();
      
      std::cout << "Result of doSegmentation: ";
      connectedThreshold->Print(std::cout, 0);
      std::cout << std::endl;
    } catch (itk::ExceptionObject & e ) {
      std::cerr << "Exception caught in doSegmentation()!" << std::endl;
      std::cerr << e << std::endl;
    }
    
    //quantification here using connectedThreshold->GetOutput() as input
    
    return connectedThreshold->GetOutput();
  }
}