//
// This example demonstrates how to read a series of dicom images
// and how to scroll with the mousewheel or the up/down keys
// through all slices
//

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
#include <vtkCamera.h>

// reading DICOM in itk
#include "itkGDCMImageIO.h"
#include "itkGDCMSeriesFileNames.h"
#include "itkImageSeriesReader.h"
#include "itkImageFileWriter.h"

class cineView
{
public:
    cineView(vtkImageData *data){
        cineView(data, 2);
    }
    
    cineView(vtkImageData *data, int axis)
    {
        this->data = data;
        
        this->mapper = vtkSmartPointer<vtkImageSliceMapper>::New();
        this->mapper->SetInputData(data);
        this->mapper->SetSliceNumber(this->mapper->GetSliceNumberMinValue());
        this->mapper->SetOrientation(axis);
        
        this->imageSlice = vtkSmartPointer<vtkImageSlice>::New();
        this->imageSlice->SetMapper(this->mapper);
        this->renderer = vtkSmartPointer<vtkRenderer>::New();
        this->renderer->AddViewProp(this->imageSlice);
        
        vtkCamera *camera = this->renderer->GetActiveCamera();
        double p[3] = {0.,0.,0.};
        p[axis]=1;
        camera->SetPosition(p);
        this->renderer->ResetCamera();
        
        if(axis==0){
            camera->Roll(-90);
        }
    }
    
    vtkSmartPointer<vtkRenderer> getRenderer(){
        return this->renderer;
    }
    
    void addAnimationObserver(vtkSmartPointer<vtkRenderWindowInteractor> interactor){
        interactor->Initialize();
        // Set up scrolling animation
        vtkSmartPointer<vtkAnimation> cb =
        vtkSmartPointer<vtkAnimation>::New();
        cb->SetMapper(this->mapper);
        interactor->AddObserver(vtkCommand::TimerEvent, cb);
        interactor->CreateRepeatingTimer(50);
    }
    
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
        
        int slice, max, min;
        vtkImageSliceMapper* mapper;
    };
    
private:
    vtkImageData *data;
    vtkSmartPointer<vtkRenderer> renderer;
    vtkSmartPointer<vtkImageSliceMapper> mapper;
    vtkSmartPointer<vtkImageSlice> imageSlice;
};
