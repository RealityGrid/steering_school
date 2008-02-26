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

TARGET=flag-viz

VTK_HOME=/opt/software/vtk-5.0.3/gcc-64

VTK_LINK=-L${VTK_HOME}/lib -lvtkCommon -lvtkFiltering -lvtkGraphics -lvtkHybrid -lvtkImaging -lvtkIO -lvtkRendering -lvtkVolumeRendering -lvtkWidgets -lvtksys -lvtkexoIIc -lvtkftgl -lvtkDICOMParser -lvtkNetCDF

REG_LINK=-L${REG_STEER_HOME}/lib64 -Wl,-Bstatic -lReG_Steer -lReG_Steer_Utils -lReG_Steer_SOAP -Wl,-Bdynamic -lxml2 -lm

CXX=g++
CPPFLAGS=-I. -I${VTK_HOME}/include/vtk-5.0 -I${REG_STEER_HOME}/include
CXXFLAGS=-Wno-deprecated -O3 -pipe
LDFLAGS=${VTK_LINK} ${REG_LINK}

# build the objects and link into the executable...

${TARGET}: flag-viz.cpp
	${CXX} -o ${TARGET} flag-viz.cpp ${CPPFLAGS} ${CXXFLAGS} ${LDFLAGS}

distclean:	clean
	rm -f *~

clean:
	rm -f ${TARGET} *.o
