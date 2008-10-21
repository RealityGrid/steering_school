# -------------------------------------------------------------------------
# (C) Copyright 2007, 2008, University of Manchester, United Kingdom,
# all rights reserved.
#
# This software was developed by the RealityGrid project
# (http://www.realitygrid.org), funded by the EPSRC under grants
# GR/R67699/01 and GR/R67699/02.
#
# LICENCE TERMS
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
# 1. Redistributions of source code must retain the above copyright
#    notice, this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright
#    notice, this list of conditions and the following disclaimer in the
#    documentation and/or other materials provided with the distribution.
#
# THIS MATERIAL IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. THE ENTIRE RISK AS TO THE QUALITY
# AND PERFORMANCE OF THE PROGRAM IS WITH YOU.  SHOULD THE PROGRAM PROVE
# DEFECTIVE, YOU ASSUME THE COST OF ALL NECESSARY SERVICING, REPAIR OR
# CORRECTION.
#
# Author: Robert Haines
# -------------------------------------------------------------------------

include ../Makefile.local

TARGET=flag-viz

VTK_LINK=-L${VTK_HOME}/lib -lvtkCommon -lvtkFiltering -lvtkGraphics -lvtkHybrid -lvtkImaging -lvtkIO -lvtkRendering -lvtkVolumeRendering -lvtkWidgets -lvtksys -lvtkexoIIc -lvtkftgl -lvtkDICOMParser -lvtkNetCDF

CPPFLAGS=-I. -I${VTK_HOME}/include/vtk-5.0
CXXFLAGS=-Wno-deprecated -O3 -pipe
LDFLAGS=${VTK_LINK}

# build the objects and link into the executable...

${TARGET}: flag-viz.cpp flag-viz-utils.cpp flag-viz.h
	${CXX} -o ${TARGET} flag-viz.cpp flag-viz-utils.cpp ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS}

distclean:	clean
	rm -f *~

clean:
	rm -f ${TARGET} *.o
