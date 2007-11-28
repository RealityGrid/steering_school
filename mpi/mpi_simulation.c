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

/* global variables */
int verbose = 1;
int num_procs;
int rank;

/* function prototypes */
void output(int, const char*, ...);
void usage(const char*);

int main(int argc, char** argv) {

  /* maximum number of 'simulation' loops to do */
  const int nloops = 5000;
  int sleep_time = 1;
  int finished = 0;


  /*
   * Steering variables to be added here
   */

  int reg_status;
  int reg_num_cmds;
  int reg_cmds[REG_INITIAL_NUM_CMDS];

  int reg_num_changed_params;
  char** reg_changed_param_labels;
  int reg_num_rcvd_cmds;
  int  reg_rcvd_cmds[REG_MAX_NUM_STR_CMDS];
  char** reg_rcvd_cmd_params;

  /*
   * End of steering variables
   */

  /* Example parameters */

  
  /* Initialise MPI */
  int mpi_status;

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

  /*
   * Initialise and enable steering library here
   */

  if(rank == 0) {
    Steering_enable(REG_TRUE);

    reg_num_cmds = 2;
    reg_cmds[0] = REG_STR_STOP;
    reg_cmds[1] = REG_STR_PAUSE_INTERNAL;
    reg_status = Steering_initialize("MPI Simulation", reg_num_cmds, reg_cmds);

    /* Use library utility routines to allocate arrays of strings
       for passing in to Steering_control */
    reg_changed_param_labels = Alloc_string_array(REG_MAX_STRING_LENGTH,
						  REG_MAX_NUM_STR_PARAMS);
    reg_rcvd_cmd_params = Alloc_string_array(REG_MAX_STRING_LENGTH,
					     REG_MAX_NUM_STR_CMDS);
    
    if(!reg_changed_param_labels || !reg_rcvd_cmd_params) {
      output(0, "Failed to allocate string arrays.\n");
      reg_status = REG_FAILURE;
    }
  }
  
  /* broadcast/collect status */
  MPI_Bcast(&reg_status, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

  /* if error, quit nicely */
  if(reg_status == REG_FAILURE) {
    MPI_Finalize();
    return REG_FAILURE;
  }

  /*
   * End of steering library initialisation
   */

  /*
   * MAIN LOOP
   */

  for(int i = 0; i < nloops; i++) {

    /* make sure all procs are at the same place */
    mpi_status = MPI_Barrier(MPI_COMM_WORLD);

    /* pretend to do some work */
    sleep(sleep_time);
    output(0, "This is loop %d\n", i);

    /*
     * Do some Steering stuff here
     */

    if(rank == 0) {
      reg_status = Steering_control(i,
				    &reg_num_changed_params,
				    reg_changed_param_labels,
				    &reg_num_rcvd_cmds,
				    reg_rcvd_cmds,
				    reg_rcvd_cmd_params);
    }

    /* broadcast/collect status */
    MPI_Bcast(&reg_status, 1, MPI_INTEGER, 0, MPI_COMM_WORLD);

    if(reg_status != REG_SUCCESS) {
      output(1, "I am %d and I detected that Steering_control failed.\n", rank);
      continue;
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

  Steering_finalize();

  /*
   * End of steering finalization
   */

  /* Finalize MPI */
  MPI_Finalize();

}

void output(int proc, const char* format, ...) {
  va_list arg_list;

  if(verbose == 1) {
    va_start(arg_list, format);

    if(proc == 0) {
      if(rank == 0) {
	vfprintf(stdout, format, arg_list);
	fflush(stdout);
      }
    }
    else {
      vfprintf(stdout, format, arg_list);
      fflush(stdout);      
    }

    va_end(arg_list);
  }
}

void usage(const char* exe) {
  printf("Usage: %s [-q]\n\n  -q - Quiet operation. No console output.\n\n", exe);
}
