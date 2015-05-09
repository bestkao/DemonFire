#include "DemonFire.h"

namespace fire {
    ImageType::Pointer doSegmentation (ImageType::Pointer input, int xSeed, int ySeed, int zSeed, int lowerTH, int upperTH){
      //the region growing filter type
      typedef itk::ConnectedThresholdImageFilter<ImageType, ImageType > ConnectedFilterType;
      
      //the region growing filter
      ConnectedFilterType::Pointer connectedThreshold = ConnectedFilterType::New();
      
      int radius = 3;
      
      //this works but it takes several minutes, can try it with doShrink?
      //connectedThreshold->SetInput(doOpening(input, radius));
      
      connectedThreshold->SetInput(input);//get rid of this if we use the one with doOpening
      
      //the uppper and lower thresholds and the seeds are hardcoded for now
      const PixelType lowerThreshold = lowerTH; //5
      const PixelType upperThreshold = upperTH; //80
      connectedThreshold->SetLower(lowerThreshold);
      connectedThreshold->SetUpper(upperThreshold);
      
      ImageType::IndexType index; //seed
      index[0] = xSeed;
      index[1] = ySeed;
      index[2] = zSeed;
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