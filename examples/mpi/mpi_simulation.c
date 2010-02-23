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

#include "ReG_Steer_Appside.h"

/*
 * An example data structure for broadcasting all steering
 * infomation in one call as opposed to many separate ones.
 * See the create_mpi_datatype method below for how to use
 * it in an MPI_Bcast.
 */
struct _steer_params {
  /* steering */
  int    status;
  int    num_rcvd_cmds;
  int    rcvd_cmds[REG_MAX_NUM_STR_CMDS];
  int    num_params_changed;

  /* monitored */
  float temperature;

  /* steered */
  int sleep_time;
  float temp_increment;
};

typedef struct _steer_params steer_params;

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
  if(argc > 1) {
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

/*
 * This method creates an MPI derived datatype for steer_params.
 * This is needed so that steer_params can be passed around by
 * MPI_Bcast in one go as opposed to many individual calls.
 */
void create_mpi_datatype(MPI_Datatype* datatype) {
  int i;
  int type_size;
  int count = 7;
  int blocklengths[7] = {1, 1, REG_MAX_NUM_STR_CMDS, 1, 1, 1, 1};
  MPI_Datatype types[7] = {MPI_INTEGER, MPI_INTEGER, MPI_INTEGER, MPI_INTEGER, MPI_REAL, MPI_INTEGER, MPI_REAL};
  MPI_Aint displacements[7];

  /* compute displacements */
  steer_params sp;
  MPI_Address(&sp.status, &displacements[0]);
  MPI_Address(&sp.num_rcvd_cmds, &displacements[1]);
  MPI_Address(&sp.rcvd_cmds, &displacements[2]);
  MPI_Address(&sp.num_params_changed, &displacements[3]);
  MPI_Address(&sp.temperature, &displacements[4]);
  MPI_Address(&sp.sleep_time, &displacements[5]);
  MPI_Address(&sp.temp_increment, &displacements[6]);

  for(i = 6; i >= 0; i--) {
    displacements[i] -= displacements[0];
  }

  MPI_Type_struct(count, blocklengths, displacements, types, datatype);
  MPI_Type_commit(datatype);
}

/*
 * This method is simply used for outputing debug messages.
 * It can be turned on or off at runtime via the verbose flag
 * and it can be set to output on all ranks or just rank 0.
 */
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
