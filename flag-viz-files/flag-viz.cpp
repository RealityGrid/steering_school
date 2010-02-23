/*
  The RCS Steering School Tutorial Exercises

  Copyright (c) 2007-2010, University of Manchester, United Kingdom.
  All rights reserved.

  This software is produced by Research Computing Services, University
  of Manchester as part of the RealityGrid project and associated
  follow on projects, funded by the EPSRC under grants GR/R67699/01,
  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
  EP/F00561X/1.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:

    * Redistributions of source code must retain the above copyright
       notice, this list of conditions and the following disclaimer.

    * Redistributions in binary form must reproduce the above
      copyright notice, this list of conditions and the following
      disclaimer in the documentation and/or other materials provided
      with the distribution.

    * Neither the name of The University of Manchester nor the names
      of its contributors may be used to endorse or promote products
      derived from this software without specific prior written
      permission.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
  POSSIBILITY OF SUCH DAMAGE.

  Author: Robert Haines
 */

#include <iostream>
#include <cstring>

#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkCallbackCommand.h"
#include "vtkCellArray.h"
#include "vtkCommand.h"
#include "vtkCylinderSource.h"
#include "vtkFloatArray.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkLookupTable.h"
#include "vtkPNGReader.h"
#include "vtkPointData.h"
#include "vtkPoints.h"
#include "vtkPolyData.h"
#include "vtkPolyDataMapper.h"
#include "vtkPolyDataNormals.h"
#include "vtkProperty.h"
#include "vtkRenderer.h"
#include "vtkRenderWindow.h"
#include "vtkRenderWindowInteractor.h"
#include "vtkScalarBarActor.h"
#include "vtkSphereSource.h"
#include "vtkTextActor.h"
#include "vtkTextProperty.h"
#include "vtkTexture.h"

#include "flag-viz.h"

#define FLAG_IMAGE "reg-flag.png"

void read_flag_file(CallbackData*);
void keyPressCallback(vtkObject*, unsigned long, void*, void*);

int main(int argc, char* argv[]) {

  int found_data = 0;
  vtkPolyData* flag = vtkPolyData::New();
  CallbackData* cdata = new CallbackData();

  // sort out arguments and create flag
  if(argc > 2) {
    found_data = 0;
  }
  else if(argc < 2) {
    found_data = read_directory("./", cdata);
  }
  else {
    found_data = read_directory(argv[1], cdata);
  }

  if(found_data == 0) {
    fprintf(stderr, "Usage: %s [directory]\n", argv[0]);
    fprintf(stderr, "  where directory must contain at least a set of");
    fprintf(stderr, " files named\n  'vertices-<number>.dat'\n");
    return 1;
  }

  // load RealityGrid logo
  vtkPNGReader* png = vtkPNGReader::New();
  png->SetFileName(FLAG_IMAGE);
  vtkTexture* logo = vtkTexture::New();
  logo->SetInputConnection(png->GetOutputPort());

  // set up internal colour table
  vtkLookupTable* colourTable = vtkLookupTable::New();
  colourTable->SetHueRange(0.70, 0.0);
  colourTable->SetRampToLinear();
  colourTable->Build();

  // create pole
  vtkCylinderSource* tube = vtkCylinderSource::New();
  tube->SetCenter(-0.5, 3.5, 0.0);
  tube->SetHeight(16.0);
  tube->CappingOn();
  tube->SetResolution(10);

  vtkSphereSource* sphere = vtkSphereSource::New();
  sphere->SetCenter(-0.5, 12.0, 0.0);
  sphere->SetRadius(1.0);
  sphere->SetThetaResolution(10);
  sphere->SetPhiResolution(10);

  vtkAppendPolyData* pole = vtkAppendPolyData::New();
  pole->AddInput(tube->GetOutput());
  pole->AddInput(sphere->GetOutput());

  // calculate normals of flag
  vtkPolyDataNormals* norms = vtkPolyDataNormals::New();
  norms->SetInput(flag);
  norms->ConsistencyOn();
  norms->SplittingOff();

  // mappers
  vtkPolyDataMapper* meshMap = vtkPolyDataMapper::New();
  meshMap->SetScalarRange(0.0, 0.1);
  meshMap->SetLookupTable(colourTable);
  meshMap->SetInputConnection(norms->GetOutputPort());

  vtkPolyDataMapper* poleMap = vtkPolyDataMapper::New();
  poleMap->SetInputConnection(pole->GetOutputPort());

  // set up actors
  vtkActor* meshActor = vtkActor::New();
  meshActor->SetMapper(meshMap);
  meshActor->SetTexture(logo);

  vtkActor* poleActor = vtkActor::New();
  poleActor->SetMapper(poleMap);
  poleActor->GetProperty()->SetColor(0.7, 0.7, 0.7);

  // set up scale
  vtkScalarBarActor* scale = vtkScalarBarActor::New();
  scale->SetLookupTable(meshMap->GetLookupTable());
  scale->SetTitle("Legend");
  scale->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  scale->GetPositionCoordinate()->SetValue(0.1, 0.05);
  scale->SetOrientationToHorizontal();
  scale->SetWidth(0.8);
  scale->SetHeight(0.12);

  // create frame text label
  vtkTextActor* text = vtkTextActor::New();
  text->SetInput("Frame: 0");
  text->GetPositionCoordinate()->SetCoordinateSystemToNormalizedViewport();
  text->GetPositionCoordinate()->SetValue(0.8, 0.95);
  text->GetTextProperty()->SetFontSize(24);
  text->GetTextProperty()->SetFontFamilyToArial();
  text->GetTextProperty()->BoldOn();
  text->GetTextProperty()->ItalicOn();

  // set up renderer
  vtkRenderer* renderer = vtkRenderer::New();
  renderer->AddActor(meshActor);
  renderer->AddActor(poleActor);
  renderer->AddActor2D(text);
  renderer->SetBackground(0.1, 0.2, 0.4);

  // set up window
  vtkRenderWindow* renderWindow = vtkRenderWindow::New();
  renderWindow->AddRenderer(renderer);
  renderWindow->SetSize(700, 700);

  // set up interactor and style
  vtkInteractorStyleSwitch* interactorStyle = vtkInteractorStyleSwitch::New();
  interactorStyle->SetCurrentStyleToTrackballCamera();

  vtkRenderWindowInteractor* reni = vtkRenderWindowInteractor::New();
  reni->SetRenderWindow(renderWindow);
  reni->SetInteractorStyle(interactorStyle);
  reni->Initialize();

  // set up callback data
  cdata->flag = flag;
  cdata->flagActor = meshActor;
  cdata->scale = scale;
  cdata->text = text;
  cdata->renderer = renderer;

  // set up and register callback on the interactor
  vtkCallbackCommand* callback = vtkCallbackCommand::New();
  callback->SetCallback(keyPressCallback);
  callback->SetClientData((void*)cdata);
  reni->AddObserver(vtkCommand::CharEvent, callback);

  // go
  read_flag_file(cdata);
  renderWindow->Render();
  reni->Start();
}

void read_flag_file(CallbackData* cdata) {
  vtkPoints* points = vtkPoints::New();
  vtkCellArray* strips = vtkCellArray::New();
  vtkFloatArray* nodeData = vtkFloatArray::New();

  vtkPolyData* data = cdata->flag;
  vtkScalarBarActor* scale = cdata->scale;
  vtkActor* flagActor = cdata->flagActor;
  vtkRenderer* renderer = cdata->renderer;
  bool haveNodedata = (cdata->nodesfiles != NULL) ? true : false;

  // try to open and read vertices file
  char* vfilename = cdata->vertsfiles[cdata->step];
  std::ifstream vfin(vfilename);
  if(!vfin) {
    std::cerr << "Could not open vertices file: " << vfilename << std::endl;
    exit(1);
  }

  // get flag dimensions
  int xDim, yDim, dDim;
  vfin >> xDim >> yDim;
  int numPoints = xDim * yDim;

  // get points
  points->Allocate(numPoints);

  float x, y, z;

  for(int j = 0; j < yDim; j++) {
    for(int i = 0; i < xDim; i++) {
      vfin >> x >> y >> z;
      points->InsertNextPoint(x, y, z);
    }
  }
  vfin.close();

  // try to open and read nodedata file
  if(haveNodedata) {
    char* nfilename = cdata->nodesfiles[cdata->step];
    std::ifstream nfin(nfilename);
    if(!nfin) {
      std::cerr << "Could not open vertices file: " << nfilename << std::endl;
      exit(1);
    }

    // throw away x and y dimensions (we know them)
    int xThrow, yThrow;
    float tuple[3];
    nfin >> xThrow >> yThrow >> dDim;

    nodeData->SetNumberOfComponents(dDim);
    nodeData->SetNumberOfTuples(numPoints);

    for(int i = 0; i < numPoints; i++) {
      for(int j = 0; j < dDim; j++) {
	nfin >> tuple[j];
      }

      nodeData->InsertTuple(i, tuple);
    }
    nfin.close();
  }
  else {
    // if no nodedata then we set this zero
    dDim = 0;
  }

  // create strips
  for(int i = 1; i < xDim; i++) {
    strips->InsertNextCell(yDim * 2);
    for(int j = 0; j < yDim; j++) {
      strips->InsertCellPoint((xDim * j) + (i - 1));
      strips->InsertCellPoint((xDim * j) + i);
    }
  }

  // build flag
  data->SetPoints(points);
  data->SetStrips(strips);

  switch(dDim) {
  case 0:
    data->GetPointData()->SetVectors(NULL);
    data->GetPointData()->SetTCoords(NULL);
    data->GetPointData()->SetScalars(NULL);
    renderer->RemoveActor(scale);
    flagActor->GetProperty()->SetColor(0.7, 0.7, 0.2);
    break;
  case 1:
    data->GetPointData()->SetVectors(NULL);
    data->GetPointData()->SetTCoords(NULL);
    data->GetPointData()->SetScalars(nodeData);
    scale->SetTitle("Force (magnitude)");
    renderer->AddActor(scale);
    break;
  case 2:
    data->GetPointData()->SetScalars(NULL);
    data->GetPointData()->SetVectors(NULL);
    data->GetPointData()->SetTCoords(nodeData);
    renderer->RemoveActor(scale);
    flagActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    break;
  default:
    data->GetPointData()->SetScalars(NULL);
    data->GetPointData()->SetTCoords(NULL);
    data->GetPointData()->SetVectors(nodeData);
    renderer->RemoveActor(scale);
    flagActor->GetProperty()->SetColor(1.0, 1.0, 1.0);
    break;
  }

  // clean up
  points->Delete();
  strips->Delete();
  nodeData->Delete();
}

void keyPressCallback(vtkObject* obj, unsigned long eid, void* cd, void* calld) {
  vtkRenderWindowInteractor* reni = vtkRenderWindowInteractor::SafeDownCast(obj);
  CallbackData* cdata = (CallbackData*) cd;
  char frame[20];

  char* sym = reni->GetKeySym();
  if((strncmp(sym, "Up", 2) == 0) || (strncmp(sym, "Ri", 2) == 0)) {
    cdata->step < cdata->max_step ? cdata->step++ : cdata->step = cdata->max_step;
  }

  if((strncmp(sym, "Do", 2) == 0) || (strncmp(sym, "Le", 2) == 0)) {
    cdata->step > 0 ? cdata->step-- : cdata->step = 0;
  }

  read_flag_file(cdata);
  sprintf(frame, "Frame: %d", cdata->step);
  cdata->text->SetInput(frame);
  reni->Render();
}
