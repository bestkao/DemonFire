/*=========================================================================
 *
 *  Copyright Insight Software Consortium
 *
 *  Licensed under the Apache License, Version 2.0 (the "License");
 *  you may not use this file except in compliance with the License.
 *  You may obtain a copy of the License at
 *
 *         http://www.apache.org/licenses/LICENSE-2.0.txt
 *
 *  Unless required by applicable law or agreed to in writing, software
 *  distributed under the License is distributed on an "AS IS" BASIS,
 *  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *  See the License for the specific language governing permissions and
 *  limitations under the License.
 *
 *=========================================================================*/
//  Software Guide : BeginLatex
//
//  The following code is an implementation of a small ITK
//  program. It tests including header files and linking with ITK
//  libraries.
//
//  Software Guide : EndLatex

// Software Guide : BeginCodeSnippet
#include "itkImage.h"
#include <iostream>
#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>

#include "vtkConeSource.h"
#include "vtkPolyDataMapper.h"
#include "vtkRenderWindow.h"
#include "vtkCamera.h"
#include "vtkActor.h"
#include "vtkRenderer.h"

int main(int argc, char **argv)
{
    typedef itk::Image< unsigned short, 3 > ImageType;
    ImageType::Pointer image = ImageType::New();
    
    // initialize GUI
    Fl_Window *window = new Fl_Window(340,180);
    Fl_Box *box = new Fl_Box(20,40,300,100,"DemonFire CT!");
    box->box(FL_UP_BOX);
    box->labelfont(FL_BOLD+FL_ITALIC);
    box->labelsize(36);
    box->labeltype(FL_SHADOW_LABEL);
    window->end();
    window->show(argc, argv);
    
    // test VTK by creating a cone (need to figure out how to integrate into fltk)
    // First we create an instance of vtkConeSource and set some of its
    // properties. The instance of vtkConeSource "cone" is part of a
    // visualization pipeline (it is a source process object); it produces data
    // (output type is vtkPolyData) which other filters may process.
    //
    vtkConeSource *cone = vtkConeSource::New();
    cone->SetHeight( 3.0 );
    cone->SetRadius( 1.0 );
    cone->SetResolution( 10 );
    
    //
    // In this example we terminate the pipeline with a mapper process object.
    // (Intermediate filters such as vtkShrinkPolyData could be inserted in
    // between the source and the mapper.)  We create an instance of
    // vtkPolyDataMapper to map the polygonal data into graphics primitives. We
    // connect the output of the cone souece to the input of this mapper.
    //
    vtkPolyDataMapper *coneMapper = vtkPolyDataMapper::New();
    coneMapper->SetInputConnection( cone->GetOutputPort() );
    
    //
    // Create an actor to represent the cone. The actor orchestrates rendering
    // of the mapper's graphics primitives. An actor also refers to properties
    // via a vtkProperty instance, and includes an internal transformation
    // matrix. We set this actor's mapper to be coneMapper which we created
    // above.
    //
    vtkActor *coneActor = vtkActor::New();
    coneActor->SetMapper( coneMapper );
    
    //
    // Create the Renderer and assign actors to it. A renderer is like a
    // viewport. It is part or all of a window on the screen and it is
    // responsible for drawing the actors it has.  We also set the background
    // color here.
    //
    vtkRenderer *ren1= vtkRenderer::New();
    ren1->AddActor( coneActor );
    ren1->SetBackground( 0.1, 0.2, 0.4 );
    
    //
    // Finally we create the render window which will show up on the screen.
    // We put our renderer into the render window using AddRenderer. We also
    // set the size to be 300 pixels by 300.
    //
    vtkRenderWindow *renWin = vtkRenderWindow::New();
    renWin->AddRenderer( ren1 );
    renWin->SetSize( 300, 300 );
    
    //
    // Now we loop over 180 degreeees and render the cone each time.
    //
    int i;
    for (i = 0; i < 180; ++i)
    {
        // render the image
        renWin->Render();
        // rotate the active camera by one degree
        ren1->GetActiveCamera()->Azimuth( 1 );
    }
    
    //
    // Free up any objects we created. All instances in VTK are deleted by
    // using the Delete() method.
    //
    cone->Delete();
    coneMapper->Delete();
    coneActor->Delete();
    ren1->Delete();
    renWin->Delete();
    
    // start the fltk event loop
    return Fl::run();
}

// Software Guide : EndCodeSnippet

//  Software Guide : BeginLatex
//
//  This code instantiates a $3D$ image\footnote{Also known as a
//  \emph{volume}.} whose pixels are represented with type \code{unsigned
//  short}. The image is then constructed and assigned to a
//  \doxygen{SmartPointer}. Although later in the text we will discuss
//  \code{SmartPointer}s in detail, for now think of it as a handle on an
//  instance of an object (see section \ref{sec:SmartPointers} for more
//  information). The \doxygen{Image} class will be described in
//  Section~\ref{sec:ImageSection}.
//
//  Software Guide : EndLatex
