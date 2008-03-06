/*-------------------------------------------------------------------------
  (C) Copyright 2007, 2008, University of Manchester, United Kingdom,
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
  int finished = 0;

  int sleep_time;
  float temperature;
  float temp_increment;

  /*
   * Steering variables to be added here
   */

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

  /* initialize parameters */
  sleep_time = 1;
  temperature = 0.0f;
  temp_increment = 0.1234f;

  /*
   * Initialise and enable steering library here
   */

  /*
   * End of steering library initialisation
   */

  /*
   * Register parameters for steering here
   */

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
    sleep(sleep_time);
    output(0, "This is loop %d\n", i);
    output(0, "temperature = %f\n", temperature);

    temperature += temp_increment;
    if(temperature < -273.0f) {
      temperature = -273.0f;
    }

    /*
     * Do some Steering stuff here
     */

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
