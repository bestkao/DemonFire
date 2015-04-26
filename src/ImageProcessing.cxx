#include "DemonFire.h"

ImageType::Pointer ImageProcessing::doPreProcessing(ImageType::Pointer input){
    return input;
}

// each processing function has two versions:
// 'doFilter(image)' applies the requested filter and returns the result
// 'doFilter(&image)' applies the requested filter and replaces the input with the result
ImageType::Pointer ImageProcessing::doSmooth(ImageType::Pointer input){
    return NULL;
}

void ImageProcessing::doSmooth(ImageType::Pointer *input){
    *input = doSmooth(*input);
    return;
}

ImageType::Pointer ImageProcessing::doSharpen(ImageType::Pointer input){
    return NULL;
}

void ImageProcessing::doSharpen(ImageType::Pointer *input){
    *input = doSharpen(*input);
    return;
}

ImageType::Pointer ImageProcessing::doBinaryThresholding(ImageType::Pointer input, int threshold){
    typedef itk::BinaryThresholdImageFilter <ImageType, ImageType>
    BinaryThresholdImageFilterType;
    
    BinaryThresholdImageFilterType::Pointer filter
        = BinaryThresholdImageFilterType::New();
    filter->SetInput(input);
    
    return filter->GetOutput();
}

void ImageProcessing::doBinaryThresholding(ImageType::Pointer *input, int threshold){
    *input = doBinaryThresholding(*input, threshold);
    return;
}

ImageType::Pointer ImageProcessing::doMask(ImageType::Pointer input, ImageType::Pointer mask){
    return NULL;
}

void ImageProcessing::doMask(ImageType::Pointer *input, ImageType::Pointer mask){
    *input = doMask(*input, mask);
    return;
}

ImageType::Pointer ImageProcessing::doClosing(ImageType::Pointer input){
    return NULL;
}

void ImageProcessing::doClosing(ImageType::Pointer *input){
    *input = doClosing(*input);
    return;
}

ImageType::Pointer ImageProcessing::doIntensityWindowing(ImageType::Pointer input){
    typedef itk::IntensityWindowingImageFilter <ImageType, ImageType> FilterType;
    typedef itk::StatisticsImageFilter<ImageType> StatisticsFilterType;
    StatisticsFilterType::Pointer statisticsFilter
    = StatisticsFilterType::New ();
    statisticsFilter->SetInput(input);
    statisticsFilter->Update();
    
    FilterType::Pointer filter = FilterType::New();
    filter->SetInput(input);
    filter->SetWindowMinimum(statisticsFilter->GetMean()-statisticsFilter->GetSigma()*2.1);
    filter->SetWindowMaximum(statisticsFilter->GetMaximum());
    filter->SetOutputMinimum(statisticsFilter->GetMinimum());
    filter->SetOutputMaximum(statisticsFilter->GetMaximum());
    filter->Update();
    return filter->GetOutput();
}

void ImageProcessing::doIntensityWindowing(ImageType::Pointer *input){
    *input = doIntensityWindowing(*input);
    return;
}
