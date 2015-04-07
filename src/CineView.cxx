// some standard vtk headers
#include <vtkSmartPointer.h>
#include <vtkRenderWindow.h>
#include <vtkRenderWindowInteractor.h>
#include <vtkRenderer.h>
#include <vtkInteractorStyleImage.h>
#include <sstream>

#include "vtkVersion.h"


#include <vtkImageMapper.h>
#include <vtkImageSliceMapper.h>
#include <vtkImageSlice.h>
#include <vtkCommand.h>
#include <vtkCamera.h>


// cineView class provides a renderer/mapper that presents axial slices of a vtkImageData object
class cineView
{
public:
    // default constructor slices along z axis
    cineView(vtkImageData *data){
        cineView(data, 2);
    }
    
    // option axis parameter slices on x or y axis
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
        
        // need a slight camera roll to maintain image orientation for axis 0
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
