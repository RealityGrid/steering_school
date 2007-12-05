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

#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "mpi.h"

#include "ReG_Steer_Appside.h"

#include "mpi_simulation.h"

/* global variables */
int verbose = 1;
int num_procs;
int rank;

/* function prototypes */
void create_mpi_datatype(MPI_Datatype*);
void output(int, const char*, ...);
void usage(const char*);

int main(int argc, char** argv) {

  /* maximum number of 'simulation' loops to do */
  const int nloops = 5000;
  int finished = 0;


  /*
   * Steering variables to be added here
   */

  int reg_num_cmds;
  int reg_cmds[REG_INITIAL_NUM_CMDS];

  steer_params reg;

  char** reg_changed_param_labels;
  char** reg_rcvd_cmd_params;

  /*
   * End of steering variables
   */
  
  /* Initialise MPI */
  MPI_Init(&argc, &argv);
  MPI_Comm_size(MPI_COMM_WORLD, &num_procs);
  MPI_Comm_rank(MPI_COMM_WORLD, &rank);

  /* Initialise simulation */
  if(argc == 2) {
    if(strncmp(argv[1], "-q", 2) == 0) {
      verbose = 0;
    }
    else {
      if(rank == 0) {
	usage(argv[0]);
      }

      MPI_Finalize();
      exit(0);
    }
  }

  /* initialize parameters and datatype */
  reg.sleep_time = 1;
  reg.temperature = 0.0f;
  reg.temp_increment = 0.1234f;

  MPI_Datatype mpi_steer_params;
  create_mpi_datatype(&mpi_steer_params);

  /*
   * Initialise and enable steering library here
   */

  if(rank == 0) {
    Steering_enable(REG_TRUE);

    reg_num_cmds = 2;
    reg_cmds[0] = REG_STR_STOP;
    reg_cmds[1] = REG_STR_PAUSE_INTERNAL;
    reg.status = Steering_initialize("MPI Simulation", reg_num_cmds, reg_cmds);

    /* Use library utility routines to allocate arrays of strings
       for passing in to Steering_control */
    reg_changed_param_labels = Alloc_string_array(REG_MAX_STRING_LENGTH,
						  REG_MAX_NUM_STR_PARAMS);
    reg_rcvd_cmd_params = Alloc_string_array(REG_MAX_STRING_LENGTH,
					     REG_MAX_NUM_STR_CMDS);
    
    if(!reg_changed_param_labels || !reg_rcvd_cmd_params) {
      output(0, "Failed to allocate string arrays.\n");
      reg.status = REG_FAILURE;
    }
  }
  
  /* broadcast/collect status */
  MPI_Bcast(&reg.status, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  /* if error, quit nicely */
  if(reg.status == REG_FAILURE) {
    MPI_Finalize();
    return REG_FAILURE;
  }

  /*
   * End of steering library initialisation
   */

  /*
   * Register parameters for steering here
   */

  if(rank == 0) {
    /* monitored parameters */
    reg.status = Register_param("Temperature (C)", REG_FALSE,
				(void*)(&reg.temperature), REG_FLOAT, "", "");

    /* steered parameters */
    reg.status = Register_param("Sleep time", REG_TRUE,
				(void*)(&reg.sleep_time), REG_INT, "1", "5");
    reg.status = Register_param("Temperature increment", REG_TRUE,
				(void*)(&reg.temp_increment), REG_FLOAT,
				"-1.0", "1.0");
  }

  /* broadcast/collect status */
  MPI_Bcast(&reg.status, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  /* if error, quit nicely */
  if(reg.status == REG_FAILURE) {
    MPI_Finalize();
    return REG_FAILURE;
  }

  /*
   * End of parameter registration
   */

  /*
   * MAIN LOOP
   */

  for(int i = 0; i < nloops; i++) {

    /* make sure all procs are at the same place */
    MPI_Barrier(MPI_COMM_WORLD);

    /* pretend to do some work */
    sleep(reg.sleep_time);
    output(0, "This is loop %d\n", i);
    output(0, "temperature = %f\n", reg.temperature);

    reg.temperature += reg.temp_increment;
    if(reg.temperature < -273.0f) {
      reg.temperature = -273.0f;
    }

    /*
     * Do some Steering stuff here
     */

    if(rank == 0) {
      reg.status = Steering_control(i,
				    &reg.num_params_changed,
				    reg_changed_param_labels,
				    &reg.num_rcvd_cmds,
				    reg.rcvd_cmds,
				    reg_rcvd_cmd_params);
    }

    /* broadcast/collect everything */
    MPI_Bcast(&reg, 1, mpi_steer_params, 0, MPI_COMM_WORLD);

    if(reg.status != REG_SUCCESS) {
      output(1, "I detected that Steering_control failed.\n");
      continue;
    }

    /* process commands received */
    for(int i = 0; i < reg.num_rcvd_cmds; i++) {
      switch(reg.rcvd_cmds[i]) {
      case REG_STR_STOP:
	output(1, "I've been told to stop.\n");
	finished = 1;
	break;
      }
    }

    /* process parameters changed */
    /* not much to do here in this simple example as the changed
     * params were already broadcast. If the changes affect anything
     * then here is where you would put that code. In this case
     * just print out the new values of the steerable parameters.
     */
    if(reg.num_params_changed > 0) {
      output(1, "%d param(s) changed.\n", reg.num_params_changed);
      output(1, "sleep_time: %d\n", reg.sleep_time);
      output(1, "temp_increment: %f\n", reg.temp_increment);
    }

    /*
     * End steering stuff
     */

    if(finished) break;
  }

  /* 
   * END MAIN LOOP
   */

  /*
   * Finalize steering library
   */

  if(rank == 0) {
    Steering_finalize();
  }

  /*
   * End of steering finalization
   */

  /* Finalize MPI */
  MPI_Finalize();

}

void create_mpi_datatype(MPI_Datatype* datatype) {
  int type_size;
  int count = PARAMS_COUNT;
  int blocklengths[PARAMS_COUNT] = {1, 1, REG_MAX_NUM_STR_CMDS, 1, 1, 1, 1, 1};
  MPI_Datatype types[PARAMS_COUNT] = {MPI_INTEGER, MPI_INTEGER, MPI_INTEGER, MPI_INTEGER, MPI_REAL, MPI_INTEGER, MPI_REAL, MPI_UB};
  MPI_Aint displacements[PARAMS_COUNT];

  displacements[0] = 0;
  for(int i = 1; i < count; i++) {
    MPI_Type_size(types[i - 1], &type_size);
    displacements[i] = displacements[i - 1] + (type_size * blocklengths[i - 1]); 
  }
  
  MPI_Type_struct(count, blocklengths, displacements, types, datatype);
  MPI_Type_commit(datatype);
}

void output(int proc, const char* format, ...) {
  va_list arg_list;

  if(verbose == 1) {
    va_start(arg_list, format);

    if(proc == 0) {
      if(rank == 0) {
	fprintf(stdout, "proc %d: ", rank);
	vfprintf(stdout, format, arg_list);
	fflush(stdout);
      }
    }
    else {
      fprintf(stdout, "proc %d: ", rank);
      vfprintf(stdout, format, arg_list);
      fflush(stdout);      
    }

    va_end(arg_list);
  }
}

void usage(const char* exe) {
  printf("Usage: %s [-q]\n\n  -q - Quiet operation. No console output.\n\n", exe);
}
