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
          Joanna Leng
 */

/*
 * INTERNATIONAL AVS CENTRE - WARRANTY DISCLAIMER
 * Please read the file DISCLAIMER for conditions associated with this file.
 * avs@iavsc.org, www.iavsc.org
 */

/* ----------------------------------------------------------------------
 * FlagRaw Module
 * ----------------------------------------------------------------------
 * Description:
 *   Flag simulation module from AVS XP_DEMO CD.
 *   Converted from Dore' to AVS/Express by I. Curington
 *
 * Author: Ian Curington
 *
 * Modification History:
 *   12 April 1995 - ianc - CREATED express version
 *   16 April 1995 - ianc - fld call reorg
 *   23 May   1995 - ianc - fld call reorg 2
 *    8 June  1995 - ianc - optimization of field creation
 *   28 Sept  1995 - ianc - renamed sqrtl to sqrt_tbl
 *   13 Feb   1996 - ianc - changed to 3-arg omethod
 *   13 May   1996 - ianc - new parameter exposed
 *   13 Dec   1996 - ianc - fixed compile warning
 *   19 Mar   1998 - Paul Lever - modified for IAC repository
 *   30 Oct   2000 - Andrew Dodd - fixed compile warnings on Windows
 *   16 Nov   2007 - Joanna Leng - adapted for RealityGrid example
 *      Feb   2008 - Robert Haines - debugging/neatening/simplification
 *
 * ----------------------------------------------------------------------
 */

/***************************************************************************
 * Copyright (C) 1992, 1993
 * by Kubota Pacific Computer Inc.  All Rights Reserved.
 * This program is a trade secret of Kubota Pacific Computer Inc. and
 * it is not to be reproduced, published, disclosed to others, copied,
 * adapted, distributed, or displayed without the prior authorization
 * of Kubota Pacific Computer Inc.  Licensee agrees to attach or embed
 * this Notice on all copies of the program, including partial copies
 * or modified versions thereof.
 ****************************************************************************/

#include "flag.h"

#ifndef NO_STEERING
#include "ReG_Steer_Appside.h"
#endif

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

/* Search for string 'NOTE' for efficiancy notes
 *
 * NOTE: Many divs can be omitted on horizontal and vertical (but not diagonal)
 * links if we assume a tight enough system that 'length d' ** will always be
 * close to 1.
 */

int main(int argc, char** argv) {

  int n;
  int main_loop_count = 0;
  int data_vec_length;
#ifndef NO_STEERING
  /*
   * Variables declared in this section are needed for steering
   *
   * Add more things here to be used in subsequent
   * steering library calls.
   */

  int status;
  int numCommands;
  int commands[REG_INITIAL_NUM_CMDS];
  int num_recvd_cmds;
  int recvd_cmds[REG_MAX_NUM_STR_CMDS];
  char** recvd_cmd_params;
  int num_params_changed;
  char** changed_param_labels;
  int icmd;

  int num_iotypes;
  int iotype_handle[REG_INITIAL_NUM_IOTYPES];
  REG_IOHandleType iohandle;

  int data_count;
  int dims[3];

  int finished = REG_FALSE;
#else
  int main_loop_max = 10000;
  int output_freq = 100;
#endif

  FlagInfo flag_info;
  int flag_reset;

  /* initialise values in flag_info */
  flag_info.sizex = SIZEX;
  flag_info.sizey = SIZEY;
  flag_info.len1 = (SIZEX * SIZEY);
  flag_info.len3 = (3 * flag_info.len1);
  flag_info.xoff = 0;
  flag_info.yoff = flag_info.len1;
  flag_info.zoff = flag_info.len1 * 2;
  flag_info.traversal_counter = 0;
  flag_info.strength = 20.0;
  flag_info.wind_direction[0] = 0.5f;
  flag_info.wind_direction[1] = 1.0f;
  flag_info.windx = 0.0;
  flag_info.windy = 0.0;
  flag_info.windz = 0.0;
  flag_info.flag_release[RELEASE_TOP] = 0;
  flag_info.flag_release[RELEASE_BOTTOM] = 0;
  flag_info.flag_color = COLOR_SOLID;

  for(n = 0; n < flag_info.len3; n++) {
    flag_info.Vertices[n] = 0.0;
    flag_info.NodeData[n] = 0.0;
    flag_info.norm[n] = 0.0;
    flag_info.rel[n] = 0.0;
    flag_info.a[n] = 0.0;
  }

  for(n = 0; n < flag_info.len1; n++) {
    flag_info.b[n] = 0.0;
    flag_info.ld[n] = 0.0;
    flag_info.sf[n] = 0.0;
  }

  flag_reset = 0;

#ifndef NO_STEERING
  /*
   * Setup the steering library here.
   *
   * Initialise the library and parameters to steer/monitor.
   */

  /* enable steering */
  Steering_enable(REG_TRUE);

  /* initialize library and register commands we want to respond to */
  numCommands = 2;
  commands[0] = REG_STR_STOP;
  commands[1] = REG_STR_PAUSE_INTERNAL;
  status = Steering_initialize("flag-sim", numCommands, commands);

  if(status != REG_SUCCESS) {
    return REG_FAILURE;
  }

  /* Allocate memory for querying library during simulation run */
  changed_param_labels = Alloc_string_array(REG_MAX_STRING_LENGTH,
					    REG_MAX_NUM_STR_PARAMS);
  recvd_cmd_params = Alloc_string_array(REG_MAX_STRING_LENGTH,
					REG_MAX_NUM_STR_CMDS);

  if(!changed_param_labels || !recvd_cmd_params) {
    printf("Failed to allocate string arrays.\n");
    return REG_FAILURE;
  }

  /* register all the parameters */
  status = Register_param("Flag Strength", REG_TRUE,
			  (void*)(&flag_info.strength),
			  REG_DBL, "6.0", "30.0");

  status = Register_param("Flag Colour", REG_TRUE,
			  (void*)(&flag_info.flag_color),
			  REG_INT, "0", "4");

  status = Register_param("Flag Release (top)", REG_TRUE,
			  (void*)(&flag_info.flag_release[0]),
			  REG_INT, "0", "1");

  status = Register_param("Flag Release (bottom)", REG_TRUE,
			  (void*)(&flag_info.flag_release[1]),
			  REG_INT, "0", "1");

  status = Register_param("Wind vector 1", REG_TRUE,
			  (void*)(&flag_info.wind_direction[0]),
			  REG_FLOAT, "0", "1");

  status = Register_param("Wind vector 2", REG_TRUE,
			  (void*)(&flag_info.wind_direction[1]),
			  REG_FLOAT, "0", "1");

  status = Register_param("Reset Flag", REG_TRUE,
			  (void*)(&flag_reset),
			  REG_INT, "0", "1");

  if(status != REG_SUCCESS){
    printf("Failed to register parameters\n");
    return REG_FAILURE;
  }

  /* register io channel to connect to visualization */
  num_iotypes = 1;
  if(Register_IOType("flag data",
		     REG_IO_OUT,
		     10,      /* Attempt to output every 10 timesteps. */
		     &(iotype_handle[0])) != REG_SUCCESS) {
    printf("Failed to register IO type \"flag data\"\n");
    Steering_finalize();
    return REG_FAILURE;
  }

#else
  /* get run time and output frequency from command args */
  if(argc > 4) {
    fprintf(stderr, "Usage: %s [flag colour] [no. loops] [output freq]", argv[0]);
    fprintf(stderr, "\n\n  where flag colour can be:");
    fprintf(stderr, "\t0 - solid colour (default),\n");
    fprintf(stderr, "\t\t\t\t1 - coloured by velocity,\n");
    fprintf(stderr, "\t\t\t\t2 - coloured by force,\n");
    fprintf(stderr, "\t\t\t\t3 - coloured by force magnitude,\n");
    fprintf(stderr, "\t\t\t\t4 - texture mapped.\n");
    exit(EXIT_FAILURE);
  }

  if(argc > 1) {
    int colour = atoi(argv[1]);
    if(colour >= 0 && colour <= 4) {
      flag_info.flag_color = colour;
    }
  }

  if(argc > 2) {
    main_loop_max = atoi(argv[2]);
  }

  if(argc > 3) {
    output_freq = atoi(argv[3]);
  }
  else {
    output_freq = (int) main_loop_max / 100;
  }
#endif

  /***********************
   * Run CFD Simulation  *
   ***********************/

  /* initialise the systems */
  init_sqrt(&flag_info);
  init_flag(&flag_info);
  calc_wind(&flag_info);
  createflag(&flag_info);

  /* get initial node data vector length */
  switch(flag_info.flag_color) {
  case COLOR_TEXTURE:
    data_vec_length = 2;
    break;
  case COLOR_FORCEMAG:
    data_vec_length = 1;
    break;
  case COLOR_VELOCITY:
  case COLOR_FORCE:
    data_vec_length = 3;
    break;
  case COLOR_SOLID:
    data_vec_length = 0;
    break;
  }

#ifndef NO_STEERING
  /* use a while-loop for indefinite run when steering */
  while(!finished) {

    if(main_loop_count % 10 == 0) {
      printf("main loop count: %d\n", main_loop_count);
    }

    /* sleep for a bit as otherwise this runs too quickly! */
    usleep(10000);

    if(flag_reset == 1) {
      /* reinitialise the systems */
      init_sqrt(&flag_info);
      init_flag(&flag_info);
      calc_wind(&flag_info);
      recreateflag(&flag_info);
      flag_reset = 0;
    }
#else
  /* use a for-loop for a bounded run when not steering */
  for(main_loop_count = 0; main_loop_count <= main_loop_max; main_loop_count++) {
#endif

    forceflag(&flag_info);
    externalforces(&flag_info);
    moveflag(&flag_info);
    recreateflag(&flag_info);

#ifndef NO_STEERING
    /*
     * Talk to the steering library here
     * and handle the results.
     *
     * There is a lot to do here!
     * (hint: Steering_control)
     */

    /* query steering library for changes/instructions */
    status = Steering_control(main_loop_count,
			      &num_params_changed,
			      changed_param_labels,
			      &num_recvd_cmds,
			      recvd_cmds,
			      recvd_cmd_params);

    if(status == REG_SUCCESS) {
      /* if params have been changed */
      if(num_params_changed > 0) {
	printf("%d params changed\n", num_params_changed);

	/* we don't have enough parameters to really worry about
	 * which have changed - assume they all have!
	 *
	 * The only ones we care about changing here are
	 * flag colour and wind vector anyway.
	 */
	switch(flag_info.flag_color) {
	case COLOR_TEXTURE:
	  data_vec_length = 2;
	  break;
	case COLOR_FORCEMAG:
	  data_vec_length = 1;
	  break;
	case COLOR_VELOCITY:
	case COLOR_FORCE:
	  data_vec_length = 3;
	  break;
	case COLOR_SOLID:
	  data_vec_length = 0;
	  break;
	}
      }

      /* if commands have been received */
      if(num_recvd_cmds > 0) {
	/* loop through them */
	for(icmd = 0; icmd < num_recvd_cmds; icmd++) {
	  switch(recvd_cmds[icmd]) {
	  case REG_STR_STOP:
	    /* we've been told to stop - do so! */
	    finished = REG_TRUE;
	    break;

	  default:
	    /* have we been told to emit data? */
	    for(n = 0; n < num_iotypes; n++) {

	      if(recvd_cmds[icmd] == iotype_handle[n]) {
		/* yes - do so! */
		status = Emit_start(iotype_handle[n], main_loop_count, &iohandle);
		if(status != REG_SUCCESS) {
		  /* not ready to emit - next loop */
		  continue;
		}

		dims[0] = flag_info.sizex;
		dims[1] = flag_info.sizey;
		dims[2] = data_vec_length;
		data_count = 3;
		status = Emit_data_slice(iohandle,
					 REG_INT,
					 data_count,
					 dims);

		data_count = flag_info.sizex * flag_info.sizey * 3;
		status = Emit_data_slice(iohandle,
					 REG_FLOAT,
					 data_count,
					 flag_info.Vertices);

		data_count = flag_info.sizex * flag_info.sizey * data_vec_length;
		if(data_vec_length > 0) {
		  status = Emit_data_slice(iohandle,
					   REG_FLOAT,
					   data_count,
					   flag_info.NodeData);
		}

		Emit_stop(&iohandle);
	      }
	    }
	    break;
	  }
	}
	if(finished) break;
      }
      if(finished) break;
    }
    else {
      printf("Call to steering control failed.\n");
    }

    /* need to increment the loop count manually */
    main_loop_count++;
#else
    /* output data files at output_freq */
    if(main_loop_count % output_freq == 0) {
      int i;
      int j;
      FILE* f_vertices;
      FILE* f_nodedata;
      char filename1[1000];
      char filename2[1000];
      int filename_pad = floor(log10(main_loop_max)) + 1;

      /* open vertices file and write */
      sprintf(filename1, "vertices-%0*2$d.dat", main_loop_count, filename_pad);
      if((f_vertices = fopen(filename1, "w")) == NULL) {
	printf("unable to open %s\n", filename1);
	exit(EXIT_FAILURE);
      }
      fprintf(f_vertices, "%d %d\n", SIZEX, SIZEY);
      j = 0;
      for(n = 0; n < (SIZEX * SIZEY); n++) {
	for(i = 0; i < 3; i++) {
	  fprintf(f_vertices, "%f ", flag_info.Vertices[j]);
	  j++;
	}
	fprintf(f_vertices, "\n");
      }
      if(fclose(f_vertices) == 1) {
	printf("unable to close %s\n", filename1);
	exit(EXIT_FAILURE);
      }

      if(data_vec_length > 0) {
	/* open nodedata file and write */
	sprintf(filename2, "nodedata-%0*2$d.dat", main_loop_count, filename_pad);
	if((f_nodedata = fopen(filename2, "w")) == NULL) {
	  printf("unable to open %s\n", filename2);
	  exit(EXIT_FAILURE);
	}
	fprintf(f_nodedata, "%d %d %d\n", SIZEX, SIZEY, data_vec_length);
	j = 0;
	for(n = 0; n < (SIZEX * SIZEY); n++) {
	  for(i = 0; i < data_vec_length; i++) {
	    fprintf(f_nodedata, "%f ", flag_info.NodeData[j]);
	    j++;
	  }
	  fprintf(f_nodedata, "\n");
	}
	if(fclose(f_nodedata) == 1) {
	  printf("unable to close %s\n", filename2);
	  exit(EXIT_FAILURE);
	}
      }

    }
#endif /* NO_STEERING */
  }

#ifndef NO_STEERING
  /*
   * Finalize steering library here!
   */

  status = Steering_finalize();
#endif

  return 0;
}
