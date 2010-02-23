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
#include <stdio.h>
#include <math.h>

/* vector operations  */

void vecmulc (
	      double a,
	      double *v,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]*=(a);
}

void vecaddc (
	      double a,
	      double *v,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]+=(a);
}

void vecsubc (
	      double a,
	      double *v,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]-=(a);
}

void vecdivc (
	      double a,
	      double *v,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]/=(a);
}

void veccopyc (
	       double a,
	       double *v,
	       int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]=(a);
}


void vecmul (
	     double *u,
	     double *v,
	     int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]*=(u)[qxg];
}

void vecadd (
	     double *u,
	     double *v,
	     int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]+=(u)[qxg];
}

void vecsub (
	     double *u,
	     double *v,
	     int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]-=(u)[qxg];
}

void vecdiv (
	     double *u,
	     double *v,
	     int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]/=(u)[qxg];
}

void veccopy (
	      double *u,
	      double *v,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (v)[qxg]=(u)[qxg];
}


void vecmulc3 (
	       double a,
	       double *v,
	       double *o,
	       int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]*(a);
}

void vecaddc3 (
	       double a,
	       double *v,
	       double *o,
	       int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]+(a);
}

void vecsubc3 (
	       double a,
	       double *v,
	       double *o,
	       int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]-(a);
}

void vecdivc3 (
	       double a,
	       double *v,
	       double *o,
	       int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]/(a);
}


void vecmul3 (
	      double *u,
	      double *v,
	      double *o,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]*(u)[qxg];
}

void vecadd3 (
	      double *u,
	      double *v,
	      double *o,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]+(u)[qxg];
}

void vecsub3 (
	      double *u,
	      double *v,
	      double *o,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]-(u)[qxg];
}

void vecdiv3 (
	      double *u,
	      double *v,
	      double *o,
	      int n)
{
  int qxg;

  for (qxg=0; qxg<n; qxg++) (o)[qxg]=(v)[qxg]/(u)[qxg];
}

/* ******************************* */
/* ***  Function Declarations  *** */
/* ******************************* */

/**************************************************************************/

void forceflag(FlagInfo *f_i)
{
  calc_wind(f_i);

  /* Start with 0 force. */

  veccopyc(0.,f_i->fxyz,f_i->len3);


  /* Essentially, we are calculating */
  /* */
  /* (strength * (l-1.)) + (FRIC * (v dot d)/l) */
  /* ------------------------------------------ * d */
  /*                     l */
  /* */
  /* where d is the vector between the two points, l is the length of d, */
  /* and v is the difference between their velocities.  This handles */
  /* spring force Fs = -k(x-x0), and friction force Ff = -Rv, where */
  /* k = strength, R = FRIC, and x0 = 1. */
  /* */
  /* Each of the following functions is specialized to calculate the */
  /* indicated forces by the above method in as vectorizable a fashion */
  /* as possible. */

  force_horizontal (f_i);
  force_vertical (f_i);
  force_diagonal (f_i);
}

/**************************************************************************/

void force_horizontal(FlagInfo *f_i)
{
  int i;

  vecsub3(f_i->pxyz,f_i->pxyz+1,f_i->dxyz,f_i->len3-1);	/* d = p1-p (vec from p to p1)  */
  vecmul3(f_i->dxyz,f_i->dxyz,f_i->txyz,f_i->len3-1);     /* t = d*d  */
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-1);	/* tx += ty  */
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-1);	/* tx += tz (tx=(length d)^2)  */

  /* No interaction between ends of flag:  */

  for (i=SIZEX-1; i<f_i->len1; i+=SIZEX)
    {f_i->txyz[i] = 1.0;}

  vecmulc(SQRTSCALE,f_i->txyz,f_i->len1-1);    /* t *= SQRTSCALE         */

  /*veci(f_i->len1-1)  */
  for(i=0;i<f_i->len1-1;i++)
    {            /* f_i->ld = length(d)         */
      if ((int)f_i->txyz[i] >= SQRTRES)
	{
	  printf("force_horizontal: (int)f_i->txyz[%d] = %d\n",i, (int)f_i->txyz[i]);
	  f_i->txyz[i] = SQRTRES - 1;
	}
      f_i->ld[i] = f_i->sqrt_tbl[(int)f_i->txyz[i]];
    }


  /* Now, f_i->ld = length(d), and we are done with f_i->txyz. */



  /* Using f_i->ld, find spring force numberator.  Leave */
  /* result in f_i->sf. */

  veccopy(f_i->ld,f_i->sf,f_i->len1-1);        /* f_i->sf = f_i->ld             */
  vecsubc(1.,f_i->sf,f_i->len1-1);             /* f_i->sf = f_i->ld-1.             */
  vecmulc(f_i->strength,f_i->sf,f_i->len1-1);  /* f_i->sf = strength * (f_i->ld-1.)     */


  /* Using f_i->ld, d, and v, find FRICtion */
  /* force numberator.  Leave result in f_i->txyz. */

  vecsub3(f_i->vxyz,f_i->vxyz+1,f_i->txyz,f_i->len3-1); /* t = v (rel velocity)     */
  vecmul(f_i->dxyz,f_i->txyz,f_i->len3-1);      /* t = v * d             */
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-1);      /* ...             */
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-1);      /* t = v dot d         */
  /*NOTE    vecmulc(FRIC,f_i->txyz,f_i->len1-1);      // t = FRIC * (v dot d)     */
  /*NOTE    vecdiv(f_i->ld,f_i->txyz,f_i->len1-1);          // t = FRIC * (v dot d) / l     */

  /* Nullify any effects at the wrap around */
  /* points. (sides of flag) */

  for (i=SIZEX-1; i<f_i->len1; i+=SIZEX)
    {f_i->txyz[i] = 0.0;}

  /* Add forces together... */

  vecadd(f_i->txyz,f_i->sf,f_i->len1-1);

  /*NOTE    vecdiv(f_i->ld,f_i->sf,f_i->len1-1); // f_i->sf = (lots of stuff)/f_i->ld     */

  vecmul(f_i->sf,f_i->dxyz+f_i->xoff,f_i->len1-1);    /* d  = force vector.         */
  vecmul(f_i->sf,f_i->dxyz+f_i->yoff,f_i->len1-1);    /* d  = force vector.         */
  vecmul(f_i->sf,f_i->dxyz+f_i->zoff,f_i->len1-1);    /* d  = force vector.         */

  /* Remember, when going from a len1 to */
  /* a len3, we must pay special attention to */
  /* what happens at the wrap around points. */


  f_i->dxyz[f_i->len1-1] = 0.0;
  f_i->dxyz[f_i->yoff+f_i->len1-1] = 0.0;

  vecadd(f_i->dxyz,f_i->fxyz,f_i->len3-1);    /* Force pulling right         */
  vecsub(f_i->dxyz,f_i->fxyz+1,f_i->len3-1);    /* Force pulling left         */
}

/**************************************************************************/


/* See force_horizontal for in code comments. */

void force_vertical (FlagInfo *f_i)
{
  int i;

  vecsub3(f_i->pxyz,f_i->pxyz+SIZEX,f_i->dxyz,f_i->len3-SIZEX);
  vecmul3(f_i->dxyz,f_i->dxyz,f_i->txyz,f_i->len3-SIZEX);
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-SIZEX);
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-SIZEX);
  vecmulc(SQRTSCALE,f_i->txyz,f_i->len1-SIZEX);    /* These two lines just set  */

  /*veci(f_i->len1-SIZEX)  */
  for(i=0;i<f_i->len1-SIZEX;i++)
    {/* f_i->ld = sqrt(f_i->txyz)   -NOTE-  */
      if ((int)f_i->txyz[i] >= SQRTRES)
	{
#ifdef DBG_MESSAGE
	  printf("force_vertical: (int)f_i->txyz[%d] = %d\n",
		 i, (int)f_i->txyz[i]);
#endif
	  f_i->txyz[i] = SQRTRES - 1;
	}
      f_i->ld[i] = f_i->sqrt_tbl[(int)f_i->txyz[i]];
    }
  veccopy(f_i->ld,f_i->sf,f_i->len1-SIZEX);
  vecsubc(1.,f_i->sf,f_i->len1-SIZEX);
  vecmulc(f_i->strength,f_i->sf,f_i->len1-SIZEX);
  vecsub3(f_i->vxyz,f_i->vxyz+SIZEX,f_i->txyz,f_i->len3-SIZEX);
  vecmul(f_i->dxyz,f_i->txyz,f_i->len3-SIZEX);
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-SIZEX);
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-SIZEX);
  /*NOTE    vecmulc(FRIC,f_i->txyz,f_i->len1-SIZEX);  */
  /*NOTE    vecdiv(f_i->ld,f_i->txyz,f_i->len1-SIZEX);   // NOTE: divide by 1.  */
  vecadd(f_i->txyz,f_i->sf,f_i->len1-SIZEX);
  /*NOTE    vecdiv(f_i->ld,f_i->sf,f_i->len1-SIZEX);     // NOTE: divide by 1.  */
  vecmul(f_i->sf,f_i->dxyz+f_i->xoff,f_i->len1-SIZEX);
  vecmul(f_i->sf,f_i->dxyz+f_i->yoff,f_i->len1-SIZEX);
  vecmul(f_i->sf,f_i->dxyz+f_i->zoff,f_i->len1-SIZEX);
  veccopyc(0.,f_i->dxyz+(f_i->len1-SIZEX),SIZEX);
  veccopyc(0.,f_i->dxyz+(f_i->yoff+(f_i->len1-SIZEX)),SIZEX);
  vecadd(f_i->dxyz,f_i->fxyz,f_i->len3-SIZEX);
  vecsub(f_i->dxyz,f_i->fxyz+SIZEX,f_i->len3-SIZEX);
}

/**************************************************************************/


/* See force_horizontal for in code comments. */

void force_diagonal (FlagInfo *f_i)
{
  int i;

  vecsub3(f_i->pxyz,f_i->pxyz+(SIZEX+1),f_i->dxyz,f_i->len3-(SIZEX+1));
  vecmul3(f_i->dxyz,f_i->dxyz,f_i->txyz,f_i->len3-(SIZEX+1));
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-(SIZEX+1));
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-(SIZEX+1));
  for (i=SIZEX-1; i<f_i->len1-(SIZEX+1); i+=SIZEX)
    {f_i->txyz[i] = 1.0;}            /* Anything safe for sqrt  */
  vecmulc(SQRTSCALE,f_i->txyz,f_i->len1-(SIZEX+1)); /* These two lines just set  */
  /*veci(f_i->len1-(SIZEX+1))  */
  for(i=0;i<f_i->len1-(SIZEX+1);i++)
    {            /* f_i->ld = sqrt(f_i->txyz)   -NOTE-  */
      if ((int)f_i->txyz[i] >= SQRTRES)
	{

	  /*            printf("force_diagonal: (int)f_i->txyz[%d] = %d\n", */
	  /*                            i, (int)f_i->txyz[i]); */

	  f_i->txyz[i] = SQRTRES - 1;
	}
      f_i->ld[i] = f_i->sqrt_tbl[(int)f_i->txyz[i]];
    }
  veccopy(f_i->ld,f_i->sf,f_i->len1-(SIZEX+1));
  vecsubc(DLEN,f_i->sf,f_i->len1-(SIZEX+1));
  vecmulc(f_i->strength,f_i->sf,f_i->len1-(SIZEX+1));
  vecsub3(f_i->vxyz,f_i->vxyz+(SIZEX+1),f_i->txyz,f_i->len3-(SIZEX+1));
  vecmul(f_i->dxyz,f_i->txyz,f_i->len3-(SIZEX+1));
  vecadd(f_i->txyz+f_i->yoff,f_i->txyz,f_i->len1-(SIZEX+1));
  vecadd(f_i->txyz+f_i->zoff,f_i->txyz,f_i->len1-(SIZEX+1));
  /*NOTE    vecmulc(FRIC,f_i->txyz,f_i->len1-(SIZEX+1));  */
  /*NOTE    vecdiv(f_i->ld,f_i->txyz,f_i->len1-(SIZEX+1));        // NOTE: divide by DLEN  */
  /*NOTE */
  vecmulc((1./DLEN),f_i->txyz,f_i->len1-(SIZEX+1));    /* NOTE: Substitute for above */
  vecadd(f_i->txyz,f_i->sf,f_i->len1-(SIZEX+1));
  /*NOTE    vecdiv(f_i->ld,f_i->sf,f_i->len1-(SIZEX+1));        // NOTE: divide by DLEN  */
  /*NOTE */
  vecmulc((1./DLEN),f_i->sf,f_i->len1-(SIZEX+1));    /* NOTE: Substitute for above */
  vecmul(f_i->sf,f_i->dxyz+f_i->xoff,f_i->len1-(SIZEX+1));
  vecmul(f_i->sf,f_i->dxyz+f_i->yoff,f_i->len1-(SIZEX+1));
  vecmul(f_i->sf,f_i->dxyz+f_i->zoff,f_i->len1-(SIZEX+1));
  veccopyc(0.,f_i->dxyz+(f_i->len1-SIZEX),SIZEX-1);
  veccopyc(0.,f_i->dxyz+(f_i->yoff+(f_i->len1-SIZEX)),SIZEX-1);
  for (i=SIZEX-1; i<f_i->len3-(SIZEX+1); i+=SIZEX)
    {f_i->dxyz[i] = 0.0;}
  vecadd(f_i->dxyz,f_i->fxyz,f_i->len3-(SIZEX+1));
  vecsub(f_i->dxyz,f_i->fxyz+(SIZEX+1),f_i->len3-(SIZEX+1));
}

/**************************************************************************/

void externalforces (FlagInfo *f_i)
{
  vecaddc ((-G), f_i->fxyz + f_i->yoff, f_i->len1);/* Force Due to Gravity  */
  force_wind (f_i);			/* Force Due to Wind     */

  /* Zero total force on attatchment points: */
  /* */
  /* To attatch all pole-side points: */
  /* */
  /* for (i=0; i<len3; i+=SIZEX) */
  /*    f_i->fxyz[i] = 0.0; */
  /* */
  /* To attattch only the corners: */


  /* Release bottom of flag.  */

  if (f_i->flag_release[RELEASE_BOTTOM]==0)
    {   f_i->fxyz [0]    = 0.0;
    f_i->fxyz [f_i->yoff] = 0.0;
    f_i->fxyz [f_i->zoff] = 0.0;
    }

  /* Release top of flag.  */

  if (f_i->flag_release[RELEASE_TOP]==0)
    {   f_i->fxyz [       (SIZEX*(SIZEY-1))] = 0.0;
    f_i->fxyz [f_i->yoff + (SIZEX*(SIZEY-1))] = 0.0;
    f_i->fxyz [f_i->zoff + (SIZEX*(SIZEY-1))] = 0.0;
    }
}

/* *************************************************************************** */
/* ** This takes the cross product of the connection to the right with the */
/* ** connection upward, to get the 'normal' of that node, which is then dotted */
/* ** with the relative wind.  (Relative wind is the absolute wind less the */
/* ** motion of that node.) That dot product is multiplied by the original */
/* ** normal vector, and added as force on that node, and perhaps the connected */
/* ** nodes.  (I haven't decided yet.) It should be noted that again assuming a */
/* ** connection length of 1., and a 90 degree connection, saves much hassle. */
/* **  */
/* ** NOTE that the wind-friction factor is assumed 1., which is to say, the */
/* ** force generated by a wind of velocity 1. (in the same units as vxyz) */
/* ** hitting a square dead on (perpendicular) will generate a perpendicular */
/* ** force of 1..  This can make the flag look dead, but it also helps make it */
/* ** look light (very affected by the wind.)  If it looks to weightless, throw */
/* ** in a vecmulc of some less than 1. number just before adding this wind */
/* ** force to the global fxyz. */
/* ***************************************************************************/


void force_wind (FlagInfo *f_i)
{
  int i;

  vecsub3(f_i->pxyz,f_i->pxyz+1,f_i->dxyz,f_i->len3-(SIZEX+1)); /* Rel vectors to right,  */
  vecsub3(f_i->pxyz,f_i->pxyz+SIZEX,f_i->txyz,f_i->len3-(SIZEX+1)); /* and up.  */


  /* Cross product: norm = f_i->dxyz X f_i->txyz */

  vecmul3(f_i->dxyz+f_i->yoff,f_i->txyz+f_i->zoff,f_i->a,f_i->len1-(SIZEX+1));
  vecmul3(f_i->dxyz+f_i->zoff,f_i->txyz+f_i->yoff,f_i->b,f_i->len1-(SIZEX+1));
  vecsub3(f_i->b,f_i->a,f_i->norm,f_i->len1-(SIZEX+1));
  vecmul3(f_i->dxyz+f_i->zoff,f_i->txyz+f_i->xoff,f_i->a,f_i->len1-(SIZEX+1));
  vecmul3(f_i->dxyz+f_i->xoff,f_i->txyz+f_i->zoff,f_i->b,f_i->len1-(SIZEX+1));
  vecsub3(f_i->b,f_i->a,f_i->norm+f_i->yoff,f_i->len1-(SIZEX+1));
  vecmul3(f_i->dxyz+f_i->xoff,f_i->txyz+f_i->yoff,f_i->a,f_i->len1-(SIZEX+1));
  vecmul3(f_i->dxyz+f_i->yoff,f_i->txyz+f_i->xoff,f_i->b,f_i->len1-(SIZEX+1));
  vecsub3(f_i->b,f_i->a,f_i->norm+f_i->zoff,f_i->len1-(SIZEX+1));

  /* Calculate rel wind/4. */
  /* (Divide by four cause we're gonna */
  /* add it to all four nodes on that */
  /* square.) */

  vecsubc3(f_i->windx,f_i->vxyz+f_i->xoff,f_i->rel+f_i->xoff,f_i->len1-(SIZEX+1));
  vecsubc3(f_i->windy,f_i->vxyz+f_i->yoff,f_i->rel+f_i->yoff,f_i->len1-(SIZEX+1));
  vecsubc3(f_i->windz,f_i->vxyz+f_i->zoff,f_i->rel+f_i->zoff,f_i->len1-(SIZEX+1));
  vecmulc(-0.25,f_i->rel,f_i->len3-(SIZEX+1));

  /* Dot product: a = norm dot rel wind/4. */

  vecmul3(f_i->norm,f_i->rel,f_i->a,f_i->len3-(SIZEX+1));
  vecadd(f_i->a+f_i->yoff,f_i->a,f_i->len1-(SIZEX+1));
  vecadd(f_i->a+f_i->zoff,f_i->a,f_i->len1-(SIZEX+1));

  /* Multiply norm by a. */

  vecmul(f_i->a,f_i->norm+f_i->xoff,f_i->len1-(SIZEX+1));
  vecmul(f_i->a,f_i->norm+f_i->yoff,f_i->len1-(SIZEX+1));
  vecmul(f_i->a,f_i->norm+f_i->zoff,f_i->len1-(SIZEX+1));

  /* Clean up the wrap crap */

  veccopyc(0.,f_i->norm+(f_i->len1-SIZEX),SIZEX-1);
  veccopyc(0.,f_i->norm+(f_i->yoff+(f_i->len1-SIZEX)),SIZEX-1);
  for (i=SIZEX-1; i<f_i->len3-(SIZEX+1); i+=SIZEX)
    {f_i->norm[i] = 0.0;}

  /* Add to the force vector. */

  vecadd(f_i->norm,f_i->fxyz,f_i->len3-(SIZEX+1));
  vecadd(f_i->norm,f_i->fxyz+1,f_i->len3-(SIZEX+1));
  vecadd(f_i->norm,f_i->fxyz+SIZEX,f_i->len3-(SIZEX+1));
  vecadd(f_i->norm,f_i->fxyz+SIZEX+1,f_i->len3-(SIZEX+1));
}


/**************************************************************************/

void moveflag (FlagInfo *f_i)
{
  vecmulc(TIMESTEP,f_i->fxyz,f_i->len3);
  vecadd(f_i->fxyz,f_i->vxyz,f_i->len3);

  vecmulc3(TIMESTEP,f_i->vxyz,f_i->dxyz,f_i->len3);
  vecadd(f_i->dxyz,f_i->pxyz,f_i->len3);
}

/**************************************************************************/

void init_sqrt (FlagInfo *f_i)
{
  int i;

  for(i=0;i<SQRTRES;i++)
    {
      f_i->sqrt_tbl[i] = sqrt(i/SQRTSCALE);
    }
}


/**************************************************************************/

void init_flag (FlagInfo *f_i)
{
  int i,xc;
  double x,y;

  xc = 0;
  x = 0.0;
  y = 0.0;

  veccopyc(0.0,f_i->vxyz,f_i->len3);

  for(i=0;i<f_i->len1;i++)
    {

      f_i->pxyz[f_i->xoff+i] = x;
      f_i->pxyz[f_i->yoff+i] = y;
      f_i->pxyz[f_i->zoff+i] = y*x*0.01/(SIZEX*SIZEY)+x*0.01/SIZEX;

      if ((++xc)>=SIZEX)
	{
	  x = 0.;
	  xc = 0;
	  y++;
	} else
	  {x++;}
    }

  f_i->flag_release [RELEASE_TOP]    = 0;
  f_i->flag_release [RELEASE_BOTTOM] = 0;
}

/**************************************************************************/

void createflag (FlagInfo *f_i)
{
  int     ix, iy;	/* Flag Vertex Indices  */
  int     index;	/* Vertex Location Index  */
  float  r_data=0.0, g_data=0.0, b_data=0.0;
  float intensity;	/* Frequency Intensity  */

  /* Texture UV coordinate tables to be initialized.  */

  for (ix=0;  ix < SIZEX;  ++ix)
    {f_i->Tu[ix] = ((float)(ix) / (float)(SIZEX-1));}

  for (iy=0;  iy < SIZEY;  ++iy)
    {f_i->Tv[iy] = ((float)(iy) / (float)(SIZEY-1));}


#ifdef DBG_MESSAGE
  index = 0;
  for (iy=0;  iy < SIZEY;  ++iy) {
    for (ix=0;  ix < SIZEX;  ++ix) {
      printf("flag: VINDEX(ix,iy) = %d %d %d\n",ix, iy,VINDEX(ix,iy));
      printf("flag: f_i->pxyz = %f %f %f\n",f_i->pxyz[index],
	     f_i->pxyz [index+f_i->yoff],
	     f_i->pxyz [index+f_i->zoff]);
      ++index;
    }
  }
#endif

  index = 0;

  if (f_i->flag_color == COLOR_TEXTURE)
    {
      for (iy=0;  iy < SIZEY;  ++iy)
	{
	  for (ix=0;  ix < SIZEX;  ++ix)
	    {
	      f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz [index];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz [index+f_i->yoff];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz [index+f_i->zoff];

	      f_i->NodeData[2 * VINDEX(ix,iy) + 0] = f_i->Tu[ix];
	      f_i->NodeData[2 * VINDEX(ix,iy) + 1] = f_i->Tv[iy];

	      ++index;
	    }
	}

      /* Graphics Draw Call with texture */

      /* return (1); */
    }

  index = 0;

  if (f_i->flag_color == COLOR_SOLID)
    {
      for (iy=0;  iy < SIZEY;  ++iy)
	{
	  for (ix=0;  ix < SIZEX;  ++ix)
	    {

	      f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz [index];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz [index+f_i->yoff];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz [index+f_i->zoff];

	      ++index;
	    }
	}
    }

  /* Color is flag force magnitude, unnormalized  */
  index = 0;
  if (f_i->flag_color == COLOR_FORCEMAG)
    {
      for (iy=0;  iy < SIZEY;  ++iy)
	{
	  for (ix=0;  ix < SIZEX;  ++ix)
	    {

	      intensity =(float)( f_i->fxyz[index]      * f_i->fxyz[index] +
				  f_i->fxyz[index+f_i->yoff] * f_i->fxyz[index+f_i->yoff] +
				  f_i->fxyz[index+f_i->zoff] * f_i->fxyz[index+f_i->zoff]);

	      f_i->NodeData[ VINDEX(ix,iy) ] = (float)sqrt( intensity );

	      f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz[index];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz[index+f_i->yoff];
	      f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz[index+f_i->zoff];

	      ++index;
	    }
	}
    }


  /* Default; Color type is COLOR_PSEUDO.  */
  index = 0;

  for (iy=0;  iy < SIZEY;  ++iy)
    {
      for (ix=0;  ix < SIZEX;  ++ix)
	{
	  f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz [index];
	  f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz [index+f_i->yoff];
	  f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz [index+f_i->zoff];

	  ++index;

	  /* Compute the vertex color based on the difference between the */
	  /* previous and current position of each vertex.  For the first */
	  /* update, set the vertex color to gray.  */

	  if (f_i->traversal_counter == 0)
	    {

	      r_data = 0.5;
	      g_data = 0.5;
	      b_data = 0.5;

	    } else {


	      if (f_i->flag_color == COLOR_VELOCITY)
		{
		  intensity = (float)(f_i->vxyz[index] * 1.5);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  r_data = (float)CLAMP(intensity, 0.0, 1.0);

		  intensity = (float)(f_i->vxyz[index+f_i->yoff] * 1.5);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  g_data = (float)CLAMP(intensity, 0.0, 1.0);

		  intensity = (float)(f_i->vxyz[index+f_i->zoff] * 1.5);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  b_data = (float)CLAMP(intensity, 0.0, 1.0);
		}
	      /* Color is flag forces  */
	      else if (f_i->flag_color == COLOR_FORCE)
		{
		  intensity = (float)(f_i->fxyz[index] * 10.0);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  r_data = (float)CLAMP(intensity, 0.0, 1.0);

		  intensity = (float)(f_i->fxyz[index+f_i->yoff] * 10.0);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  g_data = (float)CLAMP(intensity, 0.0, 1.0);

		  intensity = (float)(f_i->fxyz[index+f_i->zoff] * 10.0);
		  if (intensity < 0.0) intensity = -intensity;
		  intensity += (float)0.2;
		  b_data = (float)CLAMP(intensity, 0.0, 1.0);
		}
	    }
	  f_i->NodeData[3 * VINDEX(ix,iy) + 0] = r_data;
	  f_i->NodeData[3 * VINDEX(ix,iy) + 1] = g_data;
	  f_i->NodeData[3 * VINDEX(ix,iy) + 2] = b_data;
	}
    }

}


/**************************************************************************/

void recreateflag (FlagInfo *f_i)
{
  int     ix, iy;	/* Flag Vertex Indices  */
  int     index;	/* Vertex Location Index  */
  float  r_data=0.0, g_data=0.0, b_data=0.0;
  float intensity;	/* Frequency Intensity  */



#ifdef DBG_MESSAGE
  index = 0;
  for (iy=0;  iy < SIZEY;  ++iy) {
    for (ix=0;  ix < SIZEX;  ++ix) {
      printf("flag: VINDEX(ix,iy) = %d %d %d\n",ix, iy,VINDEX(ix,iy));
      printf("flag: f_i->pxyz = %f %f %f\n",f_i->pxyz[index],
	     f_i->pxyz [index+f_i->yoff],
	     f_i->pxyz [index+f_i->zoff]);
      ++index;
    }
  }
#endif

  switch(f_i->flag_color) {
  case COLOR_TEXTURE:
    index = 0;
    for(iy=0;  iy < SIZEY;  ++iy)
      {
	for(ix=0;  ix < SIZEX;  ++ix)
	  {

	    f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz[index];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz[index+f_i->yoff];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz[index+f_i->zoff];

	    f_i->NodeData[2 * VINDEX(ix,iy) + 0] = f_i->Tu[ix];
	    f_i->NodeData[2 * VINDEX(ix,iy) + 1] = f_i->Tv[iy];

	    ++index;
	  }
      }
    break;

  case COLOR_SOLID:
    index = 0;
    for (iy=0;  iy < SIZEY;  ++iy)
      {
	for (ix=0;  ix < SIZEX;  ++ix)
	  {
	    f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz[index];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz[index+f_i->yoff];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz[index+f_i->zoff];
	    ++index;
	  }
      }
    break;

  case COLOR_FORCEMAG:
    /* Color is flag force magnitude, unnormalized  */
    index = 0;
    for (iy=0;  iy < SIZEY;  ++iy)
      {
	for (ix=0;  ix < SIZEX;  ++ix)
	  {
	    intensity = (float)(f_i->fxyz[index]      * f_i->fxyz[index] +
				f_i->fxyz[index+f_i->yoff] * f_i->fxyz[index+f_i->yoff] +
				f_i->fxyz[index+f_i->zoff] * f_i->fxyz[index+f_i->zoff]);

	    f_i->NodeData[ VINDEX(ix,iy) ] = (float)sqrt( intensity );
	    f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz[index];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz[index+f_i->yoff];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz[index+f_i->zoff];
	    ++index;
	  }
      }
    break;

  default:
    /* Default; Color type is COLOR_PSEUDO.  */
    index = 0;

    for (iy=0;  iy < SIZEY;  ++iy)
      {
	for (ix=0;  ix < SIZEX;  ++ix)
	  {
	    f_i->Vertices[3 * VINDEX(ix,iy) + 0] = (float)f_i->pxyz[index];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 1] = (float)f_i->pxyz[index+f_i->yoff];
	    f_i->Vertices[3 * VINDEX(ix,iy) + 2] = (float)f_i->pxyz[index+f_i->zoff];

	    ++index;

	    /* Compute the vertex color based on the difference between the */
	    /* previous and current position of each vertex.  For the first */
	    /* update, set the vertex color to gray.  */

	    if (f_i->traversal_counter == 0)
	      {

		r_data = 0.5;
		g_data = 0.5;
		b_data = 0.5;

	      } else {


		if (f_i->flag_color == COLOR_VELOCITY)
		  {
		    intensity = (float)(f_i->vxyz[index] * 1.5);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    r_data = (float)CLAMP(intensity, 0.0, 1.0);

		    intensity = (float)(f_i->vxyz[index+f_i->yoff] * 1.5);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    g_data = (float)CLAMP(intensity, 0.0, 1.0);

		    intensity = (float)(f_i->vxyz[index+f_i->zoff] * 1.5);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    b_data = (float)CLAMP(intensity, 0.0, 1.0);
		  }
		/* Color is flag forces  */
		else if (f_i->flag_color == COLOR_FORCE)
		  {
		    intensity = (float)(f_i->fxyz[index] * 10.0);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    r_data = (float)CLAMP(intensity, 0.0, 1.0);

		    intensity = (float)(f_i->fxyz[index+f_i->yoff] * 10.0);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    g_data = (float)CLAMP (intensity, 0.0, 1.0);

		    intensity = (float)(f_i->fxyz[index+f_i->zoff] * 10.0);
		    if (intensity < 0.0) intensity = -intensity;
		    intensity += (float)0.2;
		    b_data = (float)CLAMP(intensity, 0.0, 1.0);
		  }
	      }
	    f_i->NodeData[3 * VINDEX(ix,iy) + 0] = (float)r_data;
	    f_i->NodeData[3 * VINDEX(ix,iy) + 1] = (float)g_data;
	    f_i->NodeData[3 * VINDEX(ix,iy) + 2] = (float)b_data;
	  }
      }
    break;
  }
}


/**************************************************************************/


void calc_wind(FlagInfo *f_i)
{
  f_i->windx = sin((f_i->wind_direction[1]+90)* PI /180)*f_i->wind_direction[0];
  f_i->windz = -cos((f_i->wind_direction[1]+90)* PI /180)*f_i->wind_direction[0];
}
