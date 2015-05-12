#include "DemonFire.h"
#include "CineView.cxx"
#include "vtkAbstractPicker.h"
#include "vtkRendererCollection.h"
#include "itkIndex.h"
using namespace fire;

// Define interaction style
class MouseInteractorStyle4 : public vtkInteractorStyleTrackballCamera
{
  public:
    static MouseInteractorStyle4* New();
    vtkTypeMacro(MouseInteractorStyle4, vtkInteractorStyleTrackballCamera);
 
    virtual void OnLeftButtonDown() 
    {
      this->Interactor->GetPicker()->Pick(this->Interactor->GetEventPosition()[0], 
                         this->Interactor->GetEventPosition()[1], 
                         0,  // always zero.
                         this->Interactor->GetRenderWindow()->GetRenderers()->GetFirstRenderer());
      double picked[3];
      this->Interactor->GetPicker()->GetPickPosition(picked);
      std::cout << "Picked value: " << picked[0] << " " << picked[1] << " " << picked[2] << std::endl;
      itk::Index<3> index;
      index[0] = picked[0]; index[1]=picked[1]; index[2]=picked[2];
      
      connector->SetInput(doSegmentation(display, picked[0], picked[1], picked[2], -1500, 3000));
      connector->Update();
      // Forward events
      vtkInteractorStyleTrackballCamera::OnLeftButtonDown();
    }
 
    virtual void OnMiddleButtonDown() 
    {
      std::cout << "Pressed middle mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnMiddleButtonDown();
    }
 
    virtual void OnRightButtonDown() 
    {
      std::cout << "Pressed right mouse button." << std::endl;
      // Forward events
      vtkInteractorStyleTrackballCamera::OnRightButtonDown();
    }
    
  public:
  ImageType::Pointer display;
  ConnectorType::Pointer connector;
 
};

vtkStandardNewMacro(MouseInteractorStyle4);

int main(int argc, char **argv){
    if(argc<3){
        std::cout << "Usage: " << argv[0];
        std::cout << " FixedFolder MovingFolder" << endl;
        return EXIT_FAILURE;
    }
    

    ImageType::Pointer image[2] = {loadDICOM(argv[1]), loadDICOM(argv[2])};
    printf("Smoothing fixed image\n");
    //image[0] = doSmoothing(image[0]);
    printf("Smoothing moving image\n");
    //image[1] = doSmoothing(image[1]);

    //doSegmentation works sort of with SegmentationImageSeries (in segmentaion file in drive)
    //Because the seed is currently hardcoded, it only works with specific series
    //int xSeed = 217;
    //int ySeed = 224;
    //int zSeed = 1;
    //image[0] = doSegmentation(image[0], xSeed, ySeed, zSeed);
    
    
    printf("Performing registration\n");
    TransformType::Pointer transform = doRegistration(image[0], image[1]);

    typedef itk::ResampleImageFilter<ImageType, ImageType> ResampleFilterType;
    ResampleFilterType::Pointer resampler = ResampleFilterType::New();
    resampler->SetInput(image[0]);
    resampler->SetTransform(transform);

    resampler->SetSize(image[0]->GetLargestPossibleRegion().GetSize());
    resampler->SetOutputOrigin(image[0]->GetOrigin());
    resampler->SetOutputSpacing(image[0]->GetSpacing());
    resampler->SetOutputDirection(image[0]->GetDirection());
    resampler->SetDefaultPixelValue(0);

    ConnectorType::Pointer connector[] = {
                            ConnectorType::New(),
                            ConnectorType::New(),
                           };
    connector[0]->SetInput(image[0]);
    connector[1]->SetInput(resampler->GetOutput());
    try {
        connector[0]->Update();
        connector[1]->Update();
    }
    catch (itk::ExceptionObject &ex)
    {
        std::cout << ex << std::endl;
        return EXIT_FAILURE;
    }

    // Setup render window containing vtk visualization (no other GUI elements for now)
    vtkSmartPointer<vtkRenderWindow> renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->SetSize(900, 600);
    
    // Setup render window interactor (defines how user interacts with visualization)
    vtkSmartPointer<vtkRenderWindowInteractor> renderWindowInteractor =
    vtkSmartPointer<vtkRenderWindowInteractor>::New();
    
    vtkSmartPointer<MouseInteractorStyle4> style =
        vtkSmartPointer<MouseInteractorStyle4>::New();
    style->display = image[0];
    style->connector = connector[0];
    renderWindowInteractor->SetInteractorStyle( style );
    renderWindowInteractor->SetRenderWindow(renderWindow);

    // Define viewport ranges
    double xmins[] = {0,.55};
    double xmaxs[] = {0.45,1.};
    for(int i = 0; i < 2; i++)
    {
        cineView *cr = new cineView(connector[i]->GetOutput());
        renderWindow->AddRenderer(cr->getRenderer());
        cr->getRenderer()->SetViewport(xmins[i],0.0,xmaxs[i],1.0);
        cr->addAnimationObserver(renderWindowInteractor);
    }
    
    // Render and start interaction
    renderWindow->Render();
    renderWindow->SetWindowName("Axial Views");
    renderWindowInteractor->Start();
    return EXIT_SUCCESS;
}
