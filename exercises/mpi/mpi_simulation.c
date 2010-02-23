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
