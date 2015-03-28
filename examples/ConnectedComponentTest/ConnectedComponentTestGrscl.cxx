//
//  ConnectedComponentTestGrscl.cxx
//
//  Takes a DICOM image as input, applies BinaryThresholdImageFilter to get binary image,
//  then labels the result using ConnectedComponentImageFilter. Shows original, binary,
//  and colormapped labeled image in viewer, outputs number of distinct objects.
//
//
//  Created by Maya Belen Gautschi on 3/25/15.
//
//

#include "itkImage.h"
#include "itkImageFileReader.h"
#include "itkLabelToRGBImageFilter.h"
#include "itkBinaryThresholdImageFilter.h"
#include "itkRelabelComponentImageFilter.h"
#include "itkLabelStatisticsImageFilter.h"
#include "itkConnectedComponentImageFilter.h"


#include "itksys/SystemTools.hxx"
#include <sstream>

#include "QuickView.h"

int main(int argc, char *argv[]) {
  if(argc < 2)
  {
    std::cerr << "Usage: " << std::endl;
    std::cerr << argv[0] << " inputImageFile [lowerThreshold] [upperThreshold] " << std::endl;
    return EXIT_FAILURE;
  }
  
  
  //default threshold values
  int lowerThreshold = 10;
  int upperThreshold = 80;
  
  if (argc > 2)
  {
    lowerThreshold = atoi(argv[2]);
  }
  if (argc > 3)
  {
    upperThreshold = atoi(argv[3]);
  }
  
  const unsigned int Dimension = 2;
  typedef unsigned char PixelType;
  typedef itk::RGBPixel<unsigned char> RGBPixelType;
  typedef itk::Image<PixelType, Dimension>  ImageType;
  typedef itk::Image<RGBPixelType, Dimension> RGBImageType;
  typedef itk::ImageFileReader<ImageType> ReaderType;
  
  ImageType::Pointer image;
  
  ReaderType::Pointer reader = ReaderType::New();
  reader->SetFileName(argv[1]);
  
  typedef itk::BinaryThresholdImageFilter <ImageType, ImageType>
  BinaryThresholdImageFilterType;
  
  //Preparing image using BinaryThresholdImageFilter
  BinaryThresholdImageFilterType::Pointer thresholdFilter
  = BinaryThresholdImageFilterType::New();
  thresholdFilter->SetInput(reader->GetOutput());
  thresholdFilter->SetLowerThreshold(lowerThreshold);
  thresholdFilter->SetUpperThreshold(upperThreshold);
  thresholdFilter->SetInsideValue(255);
  thresholdFilter->SetOutsideValue(0);
  
  image = thresholdFilter->GetOutput();
  
  typedef itk::Image<unsigned short, Dimension> OutputImageType;
  typedef itk::ConnectedComponentImageFilter<ImageType, OutputImageType> ConnectedComponentImageFilterType;
  
  //Input to ConnectedComponentImageFilterType is the output of the BinaryThresholdImageFilter
  //Labeling each distinct object in the binary image with a unique label
  ConnectedComponentImageFilterType::Pointer connected = ConnectedComponentImageFilterType::New();
  connected->SetInput(image);
  connected->Update();
  
  std::cout << "Number of objects: " << connected->GetObjectCount() << std::endl;
  
  //Applying a colormap to the labeled image
  typedef itk::LabelToRGBImageFilter<OutputImageType, RGBImageType> RGBFilterType;
  RGBFilterType::Pointer rgbFilter = RGBFilterType::New();
  rgbFilter->SetInput(connected->GetOutput());
  
  //TODO: do stuff with labeled objects
  
  QuickView viewer;
  viewer.AddImage<ImageType>(
    reader->GetOutput(), true, itksys::SystemTools::GetFilenameName(argv[1]));
  std::stringstream desc;
  desc << "Threshold\nlower = " << lowerThreshold << " upper = " << upperThreshold;
  viewer.AddImage<ImageType>(
                             image, true, desc.str());
  desc << "\nConnected Components: " << connected->GetObjectCount() << " objects";
  viewer.AddRGBImage(
                     rgbFilter->GetOutput(), true, desc.str());
  viewer.Visualize();
  
  
  return EXIT_SUCCESS;
}
