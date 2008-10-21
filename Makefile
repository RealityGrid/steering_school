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

include Makefile.local

TARGETDIR=RealityGrid-tutorial

all: tar clean

mk-flag-sim:
	cd flag-sim; ${MAKE} 

mk-flag-viz-files:
	cd flag-viz-files; ${MAKE}

mk-flag-viz-reg:
	cd flag-viz-reg; ${MAKE}

mk-mpi:
	cd mpi; ${MAKE}

tar: mk-flag-sim mk-flag-viz-files mk-flag-viz-reg mk-mpi
	mkdir -p ${TARGETDIR}/bin
	mkdir -p ${TARGETDIR}/solutions
	mv flag-sim/flag-sim ${TARGETDIR}/bin/flag-sim
	mv flag-viz-files/flag-viz ${TARGETDIR}/bin/flag-viz-files
	mv flag-viz-reg/flag-viz ${TARGETDIR}/bin/flag-viz-reg
	mv mpi/mpi_simulation ${TARGETDIR}/bin/mpi-simulation
	cp Makefile.local ${TARGETDIR}/Makefile.local
	cp Readme.txt ${TARGETDIR}/Readme.txt
	cp flag-viz-files/reg-flag.png ${TARGETDIR}/bin/reg-flag.png
	cp -a flag-sim ${TARGETDIR}/solutions/flag-sim
	cp -a flag-viz-reg ${TARGETDIR}/solutions/flag-viz-reg
	cp -a mpi ${TARGETDIR}/solutions/mpi-simulation
	cp -a exercises ${TARGETDIR}/exercises
	ln -s ../Makefile.local ${TARGETDIR}/solutions/Makefile.local
	tar czf ${TARGETDIR}.tgz ${TARGETDIR}

clean:
	cd flag-sim; ${MAKE} clean
	cd flag-viz-files; ${MAKE} clean
	cd flag-viz-reg; ${MAKE} clean
	cd mpi; ${MAKE} clean
	rm -rf ${TARGETDIR}

distclean: clean
	rm -f ${TARGETDIR}.tgz
	rm -f *~
