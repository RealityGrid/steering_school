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
*
* ----------------------------------------------------------------------
*/


// ***************************************************************************
// Copyright (C) 1992, 1993
// by Kubota Pacific Computer Inc.  All Rights Reserved.
// This program is a trade secret of Kubota Pacific Computer Inc. and
// it is not to be reproduced, published, disclosed to others, copied,
// adapted, distributed, or displayed without the prior authorization
// of Kubota Pacific Computer Inc.  Licensee agrees to attach or embed
// this Notice on all copies of the program, including partial copies
// or modified versions thereof.
// ***************************************************************************

#include "flag.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "ReG_Steer_Appside.h"
#include "time.h"



// Search for string 'NOTE' for efficiancy notes 

// NOTE: Many divs can be omitted on horizontal and vertical (but not diagonal)
// links if we assume a tight enough system that 'length d' ** will always be
// close to 1.  


//#define DBG_MESSAGE 1



/*************
***  Main ****
**************/

main()
{

  // No. of 'simulation' loops to do 
  const int nloops = 15000;

  // For steering 
  int    status;
  int    numCommands;
  int    commands[REG_INITIAL_NUM_CMDS];
  int    num_recvd_cmds;
  int    recvd_cmds[REG_MAX_NUM_STR_CMDS];
  char** recvd_cmd_params;
  int    num_params_changed;
  char** changed_param_labels;

  int   finished = REG_FALSE;
  int   icmd;

  int sleep_time=1;


  //For steering or monitoring values
  int    num_iotypes;
  int    iotype_handle[REG_INITIAL_NUM_IOTYPES];
  int    num_chktypes;
  int    chktype_handle[REG_INITIAL_NUM_IOTYPES];
  char   chk_tag[REG_MAX_STRING_LENGTH];
  REG_IOHandleType iohandle;
  int    data_type;
  int    data_count;



  // normal flag variables

  int   count; 
  int   i;
  int	n,main_loop_count=0,j;
  FILE  *f_Vertices, *f_NodeData;
  char filename1[1000],filename2[1000];


  FlagInfo flag_info;
  Steer steer;

  // initialise values in flag_info
  flag_info.sizex=SIZEX;
  flag_info.sizey=SIZEY;
  flag_info.len1=(SIZEX * SIZEY);
  flag_info.len3=(3 * flag_info.len1);
  flag_info.xoff=0;
  flag_info.yoff=flag_info.len1;
  flag_info.zoff=flag_info.len1 * 2;

  flag_info.traversal_counter=0;

  flag_info.strength=20.0;

  flag_info.windx=0.0;
  flag_info.windy=0.0;
  flag_info.windz=0.0;


  for(n=0;n<flag_info.len3;n++)
    {
      flag_info.Vertices[n]=0.0;
      flag_info.NodeData[n]=0.0;
      flag_info.norm[n]=0.0;
      flag_info.rel[n]=0.0;
      flag_info.a[n]=0.0;
    }
  for(n=0;n<flag_info.len1;n++)
    {
      flag_info.b[n]=0.0;
      flag_info.ld[n]=0.0;
      flag_info.sf[n]=0.0;
    }

  // just used for input trigger 
  count = 0;

  // initialize values in steer
  steer.light_intensities[0]=0.0;
  steer.light_intensities[1]=0.0;
  steer.light_intensities[2]=0.0;
  steer.flag_color = 1;
  steer.flag_motion = 1;
  steer.flag_release[RELEASE_TOP] = 0;
  steer.flag_release[RELEASE_BOTTOM] = 0;
  steer.flag_wind[0]=0.5;
  steer.flag_wind[1]=(float)0.10;
  steer.flag_backg_rgb[0]=0.0;
  steer.flag_backg_rgb[1]=0.0;
  steer.flag_backg_rgb[2]=0.0;
  steer.flag_reptype=0;
  steer.flag_reset = 0;
  steer.timer=0;
  steer.flag_strength=32.5;



	
  if( (f_Vertices=fopen("Vertices0.dat","w"))==NULL)
    {
      printf("unable to open f_Verices.dat\n");
      exit(EXIT_FAILURE);
    }

  if( (f_NodeData=fopen("NodeData0.dat","w"))==NULL)
    {
      printf("unable to open f_NodeData.dat\n");
      exit(EXIT_FAILURE);
    }
       


  // Initialise & enable the steering library 


  Steering_enable(REG_TRUE);

  numCommands = 2;
  commands[0] = REG_STR_STOP;
  commands[1] = REG_STR_PAUSE;
  status = Steering_initialize("flag v.1.0", numCommands, commands);

  if(status != REG_SUCCESS)
    {
      return REG_FAILURE;
    }


  // Use library utility routines to allocate arrays of strings
  // for passing in to Steering_control 
  changed_param_labels = Alloc_string_array(REG_MAX_STRING_LENGTH,
					    REG_MAX_NUM_STR_PARAMS);
  recvd_cmd_params = Alloc_string_array(REG_MAX_STRING_LENGTH,
					REG_MAX_NUM_STR_CMDS);

  if(!changed_param_labels || !recvd_cmd_params)
    {
      printf("Failed to allocate string arrays :-(\n");
      return REG_FAILURE;
    }


  // Register some parameters
  status = Register_param("TIMER", REG_FALSE,(void *)(&steer.timer),REG_INT, "0", "100000");


  status = Register_param("FLAG_STRENGTH", REG_TRUE,(void *)(&steer.flag_strength),REG_FLOAT, "6.0", "30.0");

  status = Register_param("FLAG_COLOR", REG_TRUE, (void *)(&steer.flag_color),REG_INT, "0", "4");

  status = Register_param("FLAG_RELEASE_TOP", REG_TRUE, (void *)(&steer.flag_release[0]),REG_INT, "0", "1");

  status = Register_param("FLAG_RELEASE_BOTTOM", REG_TRUE, (void *)(&steer.flag_release[1]),REG_INT, "0", "1");

  status = Register_param("FLAG_WIND_VEC1", REG_TRUE, (void *)(&steer.flag_wind[0]),REG_FLOAT, "0", "1");

  status = Register_param("FLAG_WIND_VEC2", REG_TRUE, (void *)(&steer.flag_wind[1]),REG_FLOAT, "0", "1");

  status = Register_param("FLAG_RESET", REG_TRUE, (void *)(&steer.flag_reset),REG_INT, "0", "1");

  status = Register_param("time_to_sleep", REG_TRUE, (void *)(&sleep_time),REG_INT, "0", "100");

  if(status != REG_SUCCESS){
    printf("Failed to register parameters\n");
  }


  // Register the input and output IO channels 

  if( Register_IOType("OUTPUT", 
		      REG_IO_OUT, 
		      1, // Attempt to output every timestep. 
		      &(iotype_handle[0])) != REG_SUCCESS){
    printf("Failed to register IO type OUTPUT\n");
    Steering_finalize();
    return REG_FAILURE;
  }
  /*if( Register_IOType("OUTPUT_NODEDATA",
		      REG_IO_OUT, 
		      1, // Attempt to do output every timestep. 
		      &(iotype_handle[1])) != REG_SUCCESS){
    printf("Failed to register IO type OUTPUT_NODEDATA\n");
    Steering_finalize();
    return REG_FAILURE;
    }*/
  num_iotypes = 1;






  /***********************
   * Run CFD Simulation  *
   ***********************/


  flag_info.strength = steer.flag_strength;

  //initialise the systems 
  init_sqrt(&flag_info);
  init_flag(&flag_info, &steer);
  calc_wind(&flag_info, &steer);
  createflag(&flag_info, &steer);








  for (main_loop_count=0;  main_loop_count < nloops;  main_loop_count++) 
    { 
      if(main_loop_count%10==0)
	{ 
	  printf("main loop count: %d\n",main_loop_count);
	  steer.timer=main_loop_count;
	}

      // If you want to slow down the loop
      // pretend to do some work.
      sleep(sleep_time); 

      flag_info.strength = steer.flag_strength;

      if(steer.flag_reset==1)
	{
	  //reinitialise the systems 
	  init_sqrt(&flag_info);
	  init_flag(&flag_info, &steer);
	  calc_wind(&flag_info, &steer);
	  recreateflag(&flag_info, &steer);
	  steer.flag_reset=0;
	}

      forceflag(&flag_info, &steer);
      externalforces (&flag_info, &steer);
      moveflag (&flag_info);
      recreateflag(&flag_info, &steer);


      // Talk to the steering client (if one is connected)
      status = Steering_control(i,
				&num_params_changed,
				changed_param_labels,
				&num_recvd_cmds,
				recvd_cmds,
				recvd_cmd_params);

      if(status == REG_SUCCESS)
	{
	  if(num_recvd_cmds > 0)
	    {
	      //printf("Received %d steerer cmds\n", num_recvd_cmds);
	      for(icmd=0; icmd<num_recvd_cmds; icmd++)
		{
		  switch (recvd_cmds[icmd])
		    {
		    case REG_STR_PAUSE:
		      if(Steering_pause(&num_params_changed,
					changed_param_labels,
					&num_recvd_cmds,
					recvd_cmds,
					recvd_cmd_params) != REG_SUCCESS)
			{

			  printf("Steering_pause returned error\n");
			}

		      // Reset loop to parse commands received 
		      // following the resume/stop command that 
		      // broke us out of pause 
				
		      icmd = -1;
		      break;
			      
		    case REG_STR_STOP:
		      finished = REG_TRUE;
		      break;


		      // Deal with user-defined IO types etc. 

		      for(j=0; j<num_iotypes; j++){

			if(recvd_cmds[icmd] == iotype_handle[j]){

			  printf("Some IO command received\n");

			    // We've been told to emit some data 
			    if(Emit_start(iotype_handle[j], main_loop_count, &iohandle)== REG_SUCCESS )
			      {
				data_type=REG_FLOAT;
				data_count=1;
			      
				//	if(j==0)
				//  {
				    status = Emit_data_slice(iohandle, data_type, data_count,flag_info.Vertices);
				    //  }
				/*
				if(j==1)
				  {
				    status = Emit_data_slice(iohandle, data_type, data_count,(void *)flag_info.NodeData);
				  }
				*/
				if(status != REG_SUCCESS)
				  {
				    printf("Call to Emit_data_slice failed\n");
				    Emit_stop(&iohandle);
				    continue;
				  }

			      }
			  }
			  break;
			}
		    }
		}
	      if(finished)
		{break;}
	    }
	  if(finished)
	    {break;}
	}
      else{
	printf("Call to steering control failed.\n");
      }





      if(main_loop_count==0)
	{
	  fprintf(f_Vertices,"%d %d\n",SIZEX,SIZEY);
	  j=0;
	  for(n=0;n<(SIZEX*SIZEY);n++)
	    {
	      for(i=0;i<3;i++)
		{
		  fprintf(f_Vertices,"%f ",flag_info.Vertices[j]);
		  j++;
		}
	      fprintf(f_Vertices,"\n");
	    }

	  fprintf(f_NodeData,"%d %d\n",SIZEX,SIZEY);
	  j=0;
	  for(n=0;n<(SIZEX*SIZEY);n++)
	    {
	      for(i=0;i<3;i++)
		{
		  fprintf(f_NodeData,"%f ",flag_info.NodeData[j]);
		  j++;
		}
	      fprintf(f_NodeData,"\n");
	    }

	  if(fclose(f_Vertices)==1)
	    {
	      printf("unable to close f_Vertices.dat\n");
	      exit(EXIT_FAILURE);
	    }
	  if(fclose(f_NodeData)==1)
	    {
	      printf("unable to close f_NodeData.dat\n");
	      exit(EXIT_FAILURE);
	    }
	}
		

    }//end main loop






  if(finished)
    {
      sprintf(filename1,"Vertices%d.dat",main_loop_count);
      if( (f_Vertices=fopen(filename1,"w"))==NULL)
	{
	  printf("unable to open f_Verices.dat\n");
	  exit(EXIT_FAILURE);
	}
      sprintf(filename2,"NodeData%d.dat",main_loop_count);
      if( (f_NodeData=fopen(filename2,"w"))==NULL)
	{
	  printf("unable to open f_NodeData.dat\n");
	  exit(EXIT_FAILURE);
	}

      fprintf(f_Vertices,"%d %d\n",SIZEX,SIZEY);
      j=0;
      for(n=0;n<(SIZEX*SIZEY);n++)
	{
	  for(i=0;i<3;i++)
	    {
	      fprintf(f_Vertices,"%f ",flag_info.Vertices[j]);
	      j++;
	    }
	  fprintf(f_Vertices,"\n");
	}

      fprintf(f_NodeData,"%d %d\n",SIZEX,SIZEY);
      j=0;
      for(n=0;n<(SIZEX*SIZEY);n++)
	{
	  for(i=0;i<3;i++)
	    {
	      fprintf(f_NodeData,"%f ",flag_info.NodeData[j]);
	      j++;
	    }
	  fprintf(f_NodeData,"\n");
	}

      if(fclose(f_Vertices)==1)
	{
	  printf("unable to close f_Vertices.dat\n");
	  exit(EXIT_FAILURE);
	}
      if(fclose(f_NodeData)==1)
	{
	  printf("unable to close f_NodeData.dat\n");
	  exit(EXIT_FAILURE);
	}
    }


  // Clean up the steering library
  status = Steering_finalize();

}

