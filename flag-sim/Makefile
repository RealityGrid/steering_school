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

TARGET=flag-sim

REG_LINK=-L${REG_STEER_HOME}/lib64 -Wl,-Bstatic -lReG_Steer -lReG_Steer_Utils -lReG_Steer_SOAP -Wl,-Bdynamic -lxml2 -lm

CC=gcc
CPPFLAGS=-I. -I${REG_STEER_HOME}/include
CCFLAGS=-O3 -pipe
LDFLAGS=${REG_LINK}

# build the objects and link into the executable...

${TARGET}: flag-sim.c
	${CC} -o ${TARGET} flag-sim.c flag_utils.c ${CPPFLAGS} ${CCFLAGS} ${LDFLAGS}

no-steering:
	${MAKE} CPPFLAGS="${CPPFLAGS} -DNO_STEERING"

debug:
	${MAKE} CPPFLAGS="${CPPFLAGS} -DDBG_MESSAGE"

distclean:	clean
	rm -f *~
	rm -f *.dat

clean:
	rm -f ${TARGET} *.o
