#
#  The RCS Steering School Tutorial Exercises
#
#  Copyright (c) 2007-2010, University of Manchester, United Kingdom.
#  All rights reserved.
#
#  This software is produced by Research Computing Services, University
#  of Manchester as part of the RealityGrid project and associated
#  follow on projects, funded by the EPSRC under grants GR/R67699/01,
#  GR/R67699/02, GR/T27488/01, EP/C536452/1, EP/D500028/1,
#  EP/F00561X/1.
#
#  LICENCE TERMS
#
#  Redistribution and use in source and binary forms, with or without
#  modification, are permitted provided that the following conditions
#  are met:
#
#    * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#    * Redistributions in binary form must reproduce the above
#      copyright notice, this list of conditions and the following
#      disclaimer in the documentation and/or other materials provided
#      with the distribution.
#
#    * Neither the name of The University of Manchester nor the names
#      of its contributors may be used to endorse or promote products
#      derived from this software without specific prior written
#      permission.
#
#  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
#  "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
#  LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
#  FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
#  COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
#  INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
#  BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
#  LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
#  CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
#  LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
#  ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
#  POSSIBILITY OF SUCH DAMAGE.
#
#  Author: Robert Haines

include_directories(${MPIC_INCLUDE_PATH})
add_executable(mpi-sim mpi_simulation.c)
target_link_libraries(mpi-sim
  ${MPIC_LIBRARY}
  ${MPIC_EXTRA_LIBRARY}
  ${REG_LINK_LIBRARIES}
  ${REG_EXTERNAL_LIBS}
)

# Set MPI flags for the source file and target while building
set(MPIC_COMPILE_FLAGS "-std=c99 ${MPIC_COMPILE_FLAGS}")
set_source_files_properties(
  mpi_simulation.c PROPERTIES COMPILE_FLAGS ${MPIC_COMPILE_FLAGS}
)
if(MPIC_LINK_FLAGS)
  set_target_properties(
    mpi-sim PROPERTIES LINK_FLAGS ${MPIC_LINK_FLAGS}
  )
endif(MPIC_LINK_FLAGS)

install(TARGETS
  mpi-sim
  RUNTIME DESTINATION bin
)
