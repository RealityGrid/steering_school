/*-------------------------------------------------------------------------
  (C) Copyright 2007, University of Manchester, United Kingdom,
  all rights reserved.

  This software was developed by the RealityGrid project
  (http://www.realitygrid.org), funded by the EPSRC under grants
  GR/R67699/01 and GR/R67699/02.

  LICENCE TERMS

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions
  are met:
  1. Redistributions of source code must retain the above copyright
     notice, this list of conditions and the following disclaimer.
  2. Redistributions in binary form must reproduce the above copyright
     notice, this list of conditions and the following disclaimer in the
     documentation and/or other materials provided with the distribution.

  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
  CORRECTION.

  Author: Robert Haines
---------------------------------------------------------------------------*/

#include <iostream>
#include <semaphore.h>

#include "vtkActor.h"
#include "vtkAppendPolyData.h"
#include "vtkCallbackCommand.h"
#include "vtkCellArray.h"
#include "vtkCommand.h"
#include "vtkCylinderSource.h"
#include "vtkFloatArray.h"
#include "vtkInteractorStyleSwitch.h"
#include "vtkLookupTable.h"
#include "vtkMultiThreader.h"
#include "vtkMutexLock.h"
#include "vtkObject.h"
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
#include "vtkTexture.h"

#include "ReG_Steer_Appside.h"

#define FLAG_IMAGE "reg-flag.png"

// thread data structure
struct ThreadData {
  vtkPolyData* flag;
  vtkScalarBarActor* scale;
  vtkActor* flagActor;
  vtkRenderer* renderer;
  vtkRenderWindowInteractor* interactor;
};

// program-wide variable definitions
volatile bool reRender = false;
volatile bool regLoopDone = false;
vtkMutexLock* renderLock;
vtkMutexLock* loopLock;
sem_t regDone;

void create_flag(int*, float*, float*, ThreadData*);
void renderCallback(vtkObject*, unsigned long, void*, void*);
void* regLoop(void*);

int main(int argc, char* argv[]) {

  vtkPolyData* flag = vtkPolyData::New();

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

  // set up renderer
  vtkRenderer* renderer = vtkRenderer::New();
  renderer->AddActor(meshActor);
  renderer->AddActor(poleActor);
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

  // set up and register callback on the interactor
  vtkCallbackCommand* callback = vtkCallbackCommand::New();
  callback->SetCallback(renderCallback);
  reni->AddObserver(vtkCommand::TimerEvent, callback);
  reni->CreateTimer(VTKI_TIMER_FIRST);

  // set up steering
  renderLock = vtkMutexLock::New();
  loopLock = vtkMutexLock::New();
  sem_init(&regDone, 0, 0);
  vtkMultiThreader* thread = vtkMultiThreader::New();

  ThreadData* td = new ThreadData();
  td->flag = flag;
  td->scale = scale;
  td->flagActor = meshActor;
  td->renderer = renderer;
  td->interactor = reni;
  thread->SpawnThread(regLoop, td);

  // go
  renderWindow->Render();
  reni->Start();

  // the interactor is finished so tell the loop to finish...
  loopLock->Lock();
  regLoopDone = true;
  loopLock->Unlock();
  
  // wait for it to finish...
  sem_wait(&regDone);
}

void create_flag(int* dims, float* coords, float* nodedata, ThreadData* td) {
  vtkPoints* points = vtkPoints::New();
  vtkCellArray* strips = vtkCellArray::New();
  vtkFloatArray* nodeData = vtkFloatArray::New();

  vtkPolyData* data = td->flag;
  vtkScalarBarActor* scale = td->scale;
  vtkActor* flagActor = td->flagActor;
  vtkRenderer* renderer = td->renderer;

  // get flag dimensions
  int numPoints = dims[0] * dims[1];

  // get points
  points->Allocate(numPoints);
  if(dims[2] > 0) {
    nodeData->SetNumberOfComponents(dims[2]);
    nodeData->SetNumberOfTuples(numPoints);
  }

  int index;
  int c = 0;

  for(int j = 0; j < dims[1]; j++) {
    for(int i = 0; i < dims[0]; i++) {
      index = points->InsertNextPoint(coords[c++], coords[c++], coords[c++]);
      if(dims[2] > 0)
	nodeData->InsertTuple(index, &nodedata[(index * dims[2])]);
    }
  }

  // create strips
  for(int i = 1; i < dims[0]; i++) {
    strips->InsertNextCell(dims[1] * 2);
    for(int j = 0; j < dims[1]; j++) {
      strips->InsertCellPoint((dims[0] * j) + (i - 1));
      strips->InsertCellPoint((dims[0] * j) + i);
    }
  }

  // build flag
  data->SetPoints(points);
  data->SetStrips(strips);

  switch(dims[2]) {
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

void renderCallback(vtkObject* obj, unsigned long eid, void* cd, void* calld) {
  vtkRenderWindowInteractor* reni = vtkRenderWindowInteractor::SafeDownCast(obj);
  bool render;

  renderLock->Lock();
  render = reRender;
  renderLock->Unlock();

  if(render) {
    // render via the interactor...
    reni->Render();

    // reset reRender...
    renderLock->Lock();
    reRender = false;
    renderLock->Unlock();
  }

  // reset the timer on the interactor...
  reni->CreateTimer(VTKI_TIMER_UPDATE);
}

void* regLoop(void* userData) {
  int l = 0;
  int status;
  int numParamsChanged;
  int numRecvdCmds;
  int recvdCmds[REG_MAX_NUM_STR_CMDS];
  char** changedParamLabels;
  char** recvdCmdParams;
  bool done;
  bool needRefresh = false;

  // params to be registered

  // thread data
  ThreadData* td = (ThreadData*) ((ThreadInfoStruct*) userData)->UserData;

  // allocate memory
  changedParamLabels = Alloc_string_array(REG_MAX_STRING_LENGTH,
					  REG_MAX_NUM_STR_PARAMS);
  recvdCmdParams = Alloc_string_array(REG_MAX_STRING_LENGTH,
				      REG_MAX_NUM_STR_CMDS);

  // initialise steering library
  Steering_enable(REG_TRUE);
  int cmds[] = {REG_STR_STOP};
  Steering_initialize("Flag Visualization", 1, cmds);


  // register input channel
  int ioTypeHandle;
  status = Register_IOType("INPUT", REG_IO_IN, 1, &ioTypeHandle);


  loopLock->Lock();
  done = regLoopDone;
  loopLock->Unlock();

  int ioHandle;
  int dataType;
  int dataCount;

  // place holders for data
  int dims[3];
  float* coords = NULL;
  float* nodedata = NULL;

  // go into loop until told to finish
  while(!done) {
    // sleep for 0.2 seconds
    usleep(100000);

    ///////////////////////////////////////////////////
    //
    // Add steering control and data reading stuff here
    //
    ///////////////////////////////////////////////////

    // tell the interactor to render if needs be
    if(needRefresh) {
      renderLock->Lock();
      reRender = true;
      renderLock->Unlock();
      needRefresh = false;
    }

    // see if we're done yet
    loopLock->Lock();
    done = regLoopDone;
    loopLock->Unlock();

    // update loop count for steering library
    l++;
  }

  if(coords) free(coords);

  // clean up steering library
  Steering_finalize();

  // tell main thread that this one is done
  sem_post(&regDone);

  return NULL;
}
