#ifndef FLAG_H
#define FLAG_H

//  flag.h
/***************************************************************************
Copyright (C) 1992, 1993
by Kubota Pacific Computer Inc.  All Rights Reserved.
This program is a trade secret of Kubota Pacific Computer Inc. and
it is not to be reproduced, published, disclosed to others, copied,
adapted, distributed, or displayed without the prior authorization
of Kubota Pacific Computer Inc.  Licensee agrees to attach or embed
this Notice on all copies of the program, including partial copies
or modified versions thereof.
***************************************************************************/

// ***************************************************************************
//  Makefile for mini-application
//
//  (C) Copyright 2007, University of Manchester, United Kingdom,
//  all rights reserved.
//
//  This software was developed by the RealityGrid project
//  (http://www.realitygrid.org), funded by the EPSRC under grants
//  GR/R67699/01 and GR/R67699/02.
//
//  LICENCE TERMS
//
//  Redistribution and use in source and binary forms, with or without
//  modification, are permitted provided that the following conditions
//  are met:
//  1. Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//  2. Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in the
//     documentation and/or other materials provided with the distribution.
//
//  THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
//  A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
//  AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
//  DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
//  CORRECTION.
//
//  Author........: Joanna Leng
// ***************************************************************************


    /***************************
    ***  Program Constants  ****
    ****************************/

// Dimensions of Flag 
#define SIZEX 20
#define SIZEY 12

// NOTE: statements involving fric have been commented out on the
// assumption that friction is 1.0. 

#define FRIC  1.00
#define G     0.05		// Gravity 

// Resolution of the square-root lookup table.  

#define SQRTRES   4096
#define SQRTSCALE (SQRTRES / 8.0)

// Number of seconds per sim step: 

#define TIMESTEP  0.1

#define PI 3.14159

// Length of a diagonal is NOT 1.
#define DLEN 1.414214

#define LIGHT_AMB       0
#define LIGHT_DIR       1
#define LIGHT_SPEC      2

#define RELEASE_TOP     0
#define RELEASE_BOTTOM  1

#define MOTION_NONE     0
#define MOTION_COARSE   1
#define MOTION_REG      2
#define MOTION_FINE     3

#define COLOR_SOLID     0
#define COLOR_VELOCITY  1
#define COLOR_FORCE     2
#define COLOR_FORCEMAG  3
#define COLOR_TEXTURE   4




    /***************************
    ***  Macro Definitions  ****
    ****************************/

#define VINDEX(x,y)	(((y) * SIZEX) + (x))

#define CLAMP(x,lo,hi)  (((x) < (lo)) ? (lo) : (((x) > (hi)) ? (hi) : (x)))


    /*****************************
    ***  Typedef Definitions  ****
    ******************************/

typedef struct
{
   float  light_intensities[3];
   int    flag_color;
   int    flag_motion;
   int    flag_release[2];
   float  flag_wind[2];       //min=0.0 and max=1.0 for each component
   float  flag_backg_rgb[3];
   int    flag_reptype;
   int    flag_reset;

   int    timer;

   float  flag_strength;
}
Steer;

typedef struct
{
   int	sizex;   // flag dimension x
   int	sizey;   // flag dimension y
   int len1;     // array length for scalar
   int len3;     // array length for 3D vectors
   int xoff;     // array offset for x component
   int yoff;     // array offset for y component
   int zoff;     // array offset for z component


   // Main Loop Counter for application
   int traversal_counter;

   // Elastic Strength of each spring
   double strength;

   // output structures, for graphics, once CFD solution is complete
   float Tu[SIZEX];  //for texture coords 
   float Tv[SIZEY];  //for texture coords 
   float Vertices[SIZEX * SIZEY * 3];
   float NodeData[SIZEX * SIZEY * 3];

	// Point location, velocity, force (= acceleration, as mass is 1.).  
   double pxyz[SIZEX * SIZEY * 3];  // x, y, z coord of point 
   double vxyz[SIZEX * SIZEY * 3];  // x, y, z velocity vector 
   double fxyz[SIZEX * SIZEY * 3];  // x, y, z force vector 

   // Temporary variables.  

   double dxyz[SIZEX * SIZEY * 3];
   double txyz[SIZEX * SIZEY * 3];

   double ld[SIZEX * SIZEY];    // Length of 'd' 
   double sf[SIZEX * SIZEY];    // Spring Force 

   double norm[SIZEX * SIZEY * 3];
   double rel[SIZEX * SIZEY * 3];
   double a[SIZEX * SIZEY * 3];  
   double b[SIZEX * SIZEY];

   double windx;
   double windy;
   double windz;

   // The assumption is that the point displacement from spring neutral (a
   // distance of 1.0) will never exceed 1.0.  Therefore, the sqrt must produce
   // values from 0.0 to 2.0, leaving an input range from 0 to 4.
   //
   // So, sqrt(r) = sqrt_tbl[(int)(r*((SQRTRES-1)/4.))]
   //             = sqrt_tbl[(int)(r*SQRTSCALE)]
   //
   // NOTE: with the addition of diagonal connections, the range should be
   // increased to accomodate the added length.  

   // The divisor here should be greater than the square of the
   // greatest stretched connection length expected.  

   double sqrt_tbl[SQRTRES];
}
FlagInfo;




// prototypes 

void vecmulc  (double, double *, int);
void vecaddc  (double, double *, int);
void vecsubc  (double, double *, int);
void vecdivc  (double, double *, int);
void veccopyc  (double, double *, int);
void vecmul  (double *, double *, int);
void vecadd  (double *, double *, int);
void vecsub  (double *, double *, int);
void vecdiv  (double *, double *, int);
void veccopy  (double *, double *, int);
void vecmulc3  (double, double *, double *, int);
void vecaddc3  (double, double *, double *, int);
void vecsubc3  (double, double *, double *, int);
void vecdivc3  (double, double *, double *, int);
void vecmul3  (double *, double *, double *, int);
void vecadd3  (double *, double *, double *, int);
void vecsub3  (double *, double *, double *, int);
void vecdiv3  (double *, double *, double *, int);

int make_flag_model_group (void);
void update_flag_callback (void);
void forceflag  (FlagInfo *f_i, Steer *steer);
void force_horizontal  (FlagInfo *f_i);
void force_vertical  (FlagInfo *f_i);
void force_diagonal  (FlagInfo *f_i);
void externalforces  (FlagInfo *f_i, Steer *steer);
void force_wind  (FlagInfo *f_i);
void moveflag  (FlagInfo *f_i);
void init_sqrt  (FlagInfo *f_i);
void init_wind  (void);
void init_flag  (FlagInfo *f_i, Steer *steer);
void createflag (FlagInfo *f_i, Steer *steer);
void recreateflag (FlagInfo *f_i, Steer *steer);
void calc_wind  (FlagInfo *f_i, Steer *steer);





#endif

