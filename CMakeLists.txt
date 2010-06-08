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

project(RCS_SCHOOL)

# cmake setup boiler-plate
cmake_minimum_required(VERSION 2.6)
set(CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/CMake")

# get the hostname of this machine
site_name(REG_SCHOOL_BUILD_NAME)
string(REGEX REPLACE "\\..+$" "" REG_SCHOOL_BUILD_NAME ${REG_SCHOOL_BUILD_NAME})
mark_as_advanced(REG_SCHOOL_BUILD_NAME)

# set output directories.
if(NOT EXECUTABLE_OUTPUT_PATH)
  set(EXECUTABLE_OUTPUT_PATH ${RCS_SCHOOL_BINARY_DIR}/bin CACHE INTERNAL
    "Single output directory for building all executables.")
endif(NOT EXECUTABLE_OUTPUT_PATH)

# need ReG
find_package(RealityGrid REQUIRED)
include(${RealityGrid_USE_FILE})

# need VTK
find_package(VTK REQUIRED)
include(${VTK_USE_FILE})

# test C/C++ environment
include(TestC)

# build MPI example?
option(REG_SCHOOL_BUILD_MPI "Build the MPI example?" OFF)

# configure header
configure_file(
  "${PROJECT_SOURCE_DIR}/examples/config.h.in"
  "${PROJECT_BINARY_DIR}/config.h"
  @ONLY
)
include_directories(${PROJECT_BINARY_DIR})

add_subdirectory(examples/flag-sim)
add_subdirectory(examples/flag-viz-files)
add_subdirectory(examples/flag-viz-steered)
if(REG_SCHOOL_BUILD_MPI)
  find_package(MPIC REQUIRED)
  add_subdirectory(examples/mpi)
endif(REG_SCHOOL_BUILD_MPI)

# configure Makefile.local
list(GET VTK_INCLUDE_DIRS 0 VTK_FIRST_INCLUDE_DIR)
configure_file(
  "${PROJECT_SOURCE_DIR}/Makefile.local.in"
  "${PROJECT_BINARY_DIR}/Makefile.local"
  @ONLY
)

# copy reg-flag.png to the build tree
configure_file(
  "${PROJECT_SOURCE_DIR}/examples/reg-flag.png"
  "${PROJECT_BINARY_DIR}/bin/reg-flag.png"
  COPYONLY
)

# install rules
install(FILES
  "${PROJECT_SOURCE_DIR}/Readme.txt"
  "${PROJECT_SOURCE_DIR}/Licence.txt"
  DESTINATION .
)

# work out which exercises need to be packaged
set(EX_DIRS exercises/flag-sim exercises/flag-viz-steered)
if(REG_SCHOOL_BUILD_MPI)
  set(EX_DIRS ${EX_DIRS} exercises/mpi)
endif(REG_SCHOOL_BUILD_MPI)
install(DIRECTORY ${EX_DIRS} DESTINATION exercises USE_SOURCE_PERMISSIONS)
install(FILES "${PROJECT_BINARY_DIR}/Makefile.local" DESTINATION exercises)
install(FILES "${PROJECT_SOURCE_DIR}/examples/reg-flag.png" DESTINATION bin)

# packaging rules
include(ConfigureCPack)
include(CPack)
