#include "DemonFire.h"
namespace fire {
    ImageType::Pointer doPreProcessing(ImageType::Pointer input){
        return input;
    }

    // each processing function has two versions:
    // 'doFilter(image)' applies the requested filter and returns the result
    // 'doFilter(&image)' applies the requested filter and replaces the input with the result
    ImageType::Pointer doSmooth(ImageType::Pointer input){
        return NULL;
    }

    void doSmooth(ImageType::Pointer *input){
        *input = doSmooth(*input);
        return;
    }

    ImageType::Pointer doSharpen(ImageType::Pointer input){
        return NULL;
    }

    void doSharpen(ImageType::Pointer *input){
        *input = doSharpen(*input);
        return;
    }

    ImageType::Pointer doBinaryThresholding(ImageType::Pointer input, int threshold){
        typedef itk::BinaryThresholdImageFilter <ImageType, ImageType>
        BinaryThresholdImageFilterType;
        
        BinaryThresholdImageFilterType::Pointer filter
            = BinaryThresholdImageFilterType::New();
        filter->SetInput(input);
        
        return filter->GetOutput();
    }

    void doBinaryThresholding(ImageType::Pointer *input, int threshold){
        *input = doBinaryThresholding(*input, threshold);
        return;
    }

    ImageType::Pointer doMask(ImageType::Pointer input, ImageType::Pointer mask){
        return NULL;
    }

    void doMask(ImageType::Pointer *input, ImageType::Pointer mask){
        *input = doMask(*input, mask);
        return;
    }

    ImageType::Pointer doClosing(ImageType::Pointer input){
        return NULL;
    }

    void doClosing(ImageType::Pointer *input){
        *input = doClosing(*input);
        return;
    }

    ImageType::Pointer doIntensityWindowing(ImageType::Pointer input){
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

    void doIntensityWindowing(ImageType::Pointer *input){
        *input = doIntensityWindowing(*input);
        return;
    }
}