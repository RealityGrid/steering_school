# - Message Passing Interface (MPI) for C module.
#
# The Message Passing Interface (MPI) is a library used to write
# high-performance parallel applications that use message passing, and
# is typically deployed on a cluster. MPI is a standard interface
# (defined by the MPI forum) for which many implementations are
# available. All of these implementations have somewhat different
# compilation approaches (different include paths, libraries to link
# against, etc.), and this module tries to smooth out those differences.
#
# This module will set the following variables:
#   MPIC_FOUND                 TRUE if we have found MPI for C
#   MPIC_COMPILE_FLAGS         Compilation flags for C MPI programs
#   MPIC_INCLUDE_PATH          Include path(s) for C MPI header
#   MPIC_LINK_FLAGS            Linking flags for C MPI programs
#   MPIC_LIBRARY               First C MPI library to link (cached)
#   MPIC_EXTRA_LIBRARY         Extra C MPI libraries to link (cached)
#   MPIC_LIBRARIES             All libraries to link C MPI programs
#
# This module will attempt to auto-detect these settings, first by looking
# for a MPI C compiler, which many MPI implementations provide as a
# pass-through to the native compiler to simplify the compilation of MPI
# programs. The MPI compiler is stored in the cache variable MPIC_COMPILER,
# and will attempt to look for commonly-named drivers mpifortran, mpif95,
# mpif90, or mpif77.  If the compiler driver is found and recognized, it
# will be used to set all of the module variables. To skip this
# auto-detection, set MPIC_LIBRARY and MPIC_INCLUDE_PATH in the CMake
# cache.
#
# If no compiler driver is found or the compiler driver is not
# recognized, this module will then search for common include paths
# and library names to try to detect MPI.
#
# If CMake initially finds a different MPI than was intended, and you
# want to use the MPI compiler auto-detection for a different MPI
# implementation, set MPIC_COMPILER to the MPI compiler driver you want
# to use (e.g., mpicc) and then set MPIC_LIBRARY to the string
# MPIC_LIBRARY-NOTFOUND. When you re-configure, auto-detection of MPI
# will run again with the newly-specified MPIC_COMPILER.
#
# This module implicitly also loads the FindMPI module (the one for C).
# Thus, useful variables like MPIEXEC, MPIEXEC_NUMPROC_FLAG,
# MPIEXEC_PREFLAGS, and MPIEXEC_POSTFLAGS are also defined.  See the
# documentation for the FindMPI module for more information.
#
# When using MPIEXEC to execute MPI applications, you should typically
# use all of the MPIEXEC flags as follows:
#   ${MPIEXEC} ${MPIEXEC_NUMPROC_FLAG} PROCS ${MPIEXEC_PREFLAGS} EXECUTABLE
#     ${MPIEXEC_POSTFLAGS} ARGS
# where PROCS is the number of processors on which to execute the program,
# EXECUTABLE is the MPI program, and ARGS are the arguments to pass to the
# MPI program.

#=============================================================================
# Copyright 2001-2009 Kitware, Inc.
#
# Distributed under the OSI-approved BSD License (the "License");
# see accompanying file Copyright.txt for details.
#
# This software is distributed WITHOUT ANY WARRANTY; without even the
# implied warranty of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
# See the License for more information.
#=============================================================================
# (To distributed this file outside of CMake, substitute the full
#  License text for the above reference.)

# This module is maintained by David Partyka <dave.partyka@kitware.com>.

# A set of directories to search through in addition to the standard system paths
# that find_program will search through.
# Microsoft HPC SDK is automatically added to the system path
# Argonne National Labs MPICH2 sets a registry key that we can use.

find_package(MPI QUIET)

set(_MPIC_PACKAGE_DIR
  mpi
  mpich
  openmpi
  lib/mpi
  lib/mpich
  lib/openmpi
  "MPICH/SDK"
  "Microsoft Compute Cluster Pack"
  )

set(_MPIC_PREFIX_PATH)
if(WIN32)
  list(APPEND _MPIC_PREFIX_PATH "[HKEY_LOCAL_MACHINE\\SOFTWARE\\MPICH\\SMPD;binary]/..")
endif()

foreach(SystemPrefixDir ${CMAKE_SYSTEM_PREFIX_PATH})
  foreach(MpiPackageDir ${_MPIC_PREFIX_PATH})
    if(EXISTS ${SystemPrefixDir}/${MpiPackageDir})
      list(APPEND _MPIC_PREFIX_PATH "${SystemPrefixDir}/${MpiPackageDir}")
    endif()
  endforeach(MpiPackageDir)
endforeach(SystemPrefixDir)

# call get_filename_component twice to remove mpiexec and the directory it exists in (typically bin).
# This gives us a fairly reliable base directory to search for /bin /lib and /include from.
get_filename_component(_MPIC_BASE_DIR "${MPIEXEC}" PATH)
get_filename_component(_MPIC_BASE_DIR "${_MPIC_BASE_DIR}" PATH)

# If there is an mpi compiler find it and interogate (farther below) it for the include
# and lib dirs otherwise we will continue to search from ${_MPIC_BASE_DIR}.
find_program(MPIC_COMPILER
  NAMES openmpicc mpicc
  HINTS "${_MPIC_BASE_DIR}"
  PATH_SUFFIXES bin
  DOC "MPI C compiler. Used only to detect MPI compilation flags.")
mark_as_advanced(MPIC_COMPILER)

if (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)
  # Do nothing: we already have MPIC_INCLUDE_PATH and MPIC_LIBRARY in
  # the cache, and we don't want to override those settings.
elseif (MPIC_COMPILER)
  # Check whether the -showme:compile option works. This indicates
  # that we have either Open MPI or a newer version of LAM-MPI, and
  # implies that -showme:link will also work.
  # Note that Windows distros do not have an mpi compiler to interogate.
  exec_program(${MPIC_COMPILER}
    ARGS -showme:compile
    OUTPUT_VARIABLE MPIC_COMPILE_CMDLINE
    RETURN_VALUE MPIC_COMPILER_RETURN)

  if (MPIC_COMPILER_RETURN EQUAL 0)
    # If we appear to have -showme:compile, then we should also have
    # -showme:link. Try it.
    exec_program(${MPIC_COMPILER}
      ARGS -showme:link
      OUTPUT_VARIABLE MPIC_LINK_CMDLINE
      RETURN_VALUE MPIC_COMPILER_RETURN)

    # Note that we probably have -showme:incdirs and -showme:libdirs
    # as well.
    set(MPIC_COMPILER_MAY_HAVE_INCLIBDIRS TRUE)
  endif (MPIC_COMPILER_RETURN EQUAL 0)

  if (MPIC_COMPILER_RETURN EQUAL 0)
    # Do nothing: we have our command lines now
  else (MPIC_COMPILER_RETURN EQUAL 0)
    # Older versions of LAM-MPI have "-showme". Try it.
    exec_program(${MPIC_COMPILER}
      ARGS -showme
      OUTPUT_VARIABLE MPIC_COMPILE_CMDLINE
      RETURN_VALUE MPIC_COMPILER_RETURN)
  endif (MPIC_COMPILER_RETURN EQUAL 0)

  if (MPIC_COMPILER_RETURN EQUAL 0)
    # Do nothing: we have our command lines now
  else (MPIC_COMPILER_RETURN EQUAL 0)
    # MPICH uses "-show". Try it.
    exec_program(${MPIC_COMPILER}
      ARGS -show
      OUTPUT_VARIABLE MPIC_COMPILE_CMDLINE
      RETURN_VALUE MPIC_COMPILER_RETURN)
  endif (MPIC_COMPILER_RETURN EQUAL 0)

  if (MPIC_COMPILER_RETURN EQUAL 0)
    # We have our command lines, but we might need to copy
    # MPIC_COMPILE_CMDLINE into MPIC_LINK_CMDLINE, if the underlying
    if (NOT MPIC_LINK_CMDLINE)
      SET(MPIC_LINK_CMDLINE ${MPIC_COMPILE_CMDLINE})
    endif (NOT MPIC_LINK_CMDLINE)
  else (MPIC_COMPILER_RETURN EQUAL 0)
    message(STATUS "Unable to determine MPI from MPI driver ${MPIC_COMPILER}")
  endif (MPIC_COMPILER_RETURN EQUAL 0)
endif (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)

if (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)
  # Do nothing: we already have MPIC_INCLUDE_PATH and MPIC_LIBRARY in
  # the cache, and we don't want to override those settings.
elseif (MPIC_COMPILE_CMDLINE)
  # Extract compile flags from the compile command line.
  string(REGEX MATCHALL "-D([^\" ]+|\"[^\"]+\")" MPIC_ALL_COMPILE_FLAGS "${MPIC_COMPILE_CMDLINE}")
  set(MPIC_COMPILE_FLAGS_WORK)
  foreach(FLAG ${MPIC_ALL_COMPILE_FLAGS})
    if (MPIC_COMPILE_FLAGS_WORK)
      set(MPIC_COMPILE_FLAGS_WORK "${MPIC_COMPILE_FLAGS_WORK} ${FLAG}")
    else(MPIC_COMPILE_FLAGS_WORK)
      set(MPIC_COMPILE_FLAGS_WORK ${FLAG})
    endif(MPIC_COMPILE_FLAGS_WORK)
  endforeach(FLAG)

  # Extract include paths from compile command line
  string(REGEX MATCHALL "-I([^\" ]+|\"[^\"]+\")" MPIC_ALL_INCLUDE_PATHS "${MPIC_COMPILE_CMDLINE}")
  set(MPIC_INCLUDE_PATH_WORK)
  foreach(IPATH ${MPIC_ALL_INCLUDE_PATHS})
    string(REGEX REPLACE "^-I" "" IPATH ${IPATH})
    string(REGEX REPLACE "//" "/" IPATH ${IPATH})
    list(APPEND MPIC_INCLUDE_PATH_WORK ${IPATH})
  endforeach(IPATH)

  if (NOT MPIC_INCLUDE_PATH_WORK)
    if (MPIC_COMPILER_MAY_HAVE_INCLIBDIRS)
      # The compile command line didn't have any include paths on it,
      # but we may have -showme:incdirs. Use it.
      exec_program(${MPIC_COMPILER}
        ARGS -showme:incdirs
        OUTPUT_VARIABLE MPIC_INCLUDE_PATH_WORK
        RETURN_VALUE MPIC_COMPILER_RETURN)
      separate_arguments(MPIC_INCLUDE_PATH_WORK)
    endif (MPIC_COMPILER_MAY_HAVE_INCLIBDIRS)
  endif (NOT MPIC_INCLUDE_PATH_WORK)

  if (NOT MPIC_INCLUDE_PATH_WORK)
    # If all else fails, just search for mpif.h in the normal include
    # paths.
    find_path(MPIC_INCLUDE_PATH mpif.h
  HINTS ${_MPIC_BASE_DIR} ${_MPIC_PREFIX_PATH}
  PATH_SUFFIXES include
    )
    set(MPIC_INCLUDE_PATH_WORK ${MPIC_INCLUDE_PATH})
  endif (NOT MPIC_INCLUDE_PATH_WORK)

  # Extract linker paths from the link command line
  string(REGEX MATCHALL "-L([^\" ]+|\"[^\"]+\")" MPIC_ALL_LINK_PATHS "${MPIC_LINK_CMDLINE}")
  set(MPIC_LINK_PATH)
  foreach(LPATH ${MPIC_ALL_LINK_PATHS})
    string(REGEX REPLACE "^-L" "" LPATH ${LPATH})
    string(REGEX REPLACE "//" "/" LPATH ${LPATH})
    list(APPEND MPIC_LINK_PATH ${LPATH})
  endforeach(LPATH)

  if (NOT MPIC_LINK_PATH)
    if (MPIC_COMPILER_MAY_HAVE_INCLIBDIRS)
      # The compile command line didn't have any linking paths on it,
      # but we may have -showme:libdirs. Use it.
      exec_program(${MPIC_COMPILER}
        ARGS -showme:libdirs
        OUTPUT_VARIABLE MPIC_LINK_PATH
        RETURN_VALUE MPIC_COMPILER_RETURN)
      separate_arguments(MPIC_LINK_PATH)
    endif (MPIC_COMPILER_MAY_HAVE_INCLIBDIRS)
  endif (NOT MPIC_LINK_PATH)

  # Extract linker flags from the link command line
  string(REGEX MATCHALL "-Wl,([^\" ]+|\"[^\"]+\")" MPIC_ALL_LINK_FLAGS "${MPIC_LINK_CMDLINE}")
  set(MPIC_LINK_FLAGS_WORK)
  foreach(FLAG ${MPIC_ALL_LINK_FLAGS})
    if (MPIC_LINK_FLAGS_WORK)
      set(MPIC_LINK_FLAGS_WORK "${MPIC_LINK_FLAGS_WORK} ${FLAG}")
    else(MPIC_LINK_FLAGS_WORK)
      set(MPIC_LINK_FLAGS_WORK ${FLAG})
    endif(MPIC_LINK_FLAGS_WORK)
  endforeach(FLAG)

  # Extract the set of libraries to link against from the link command
  # line
  string(REGEX MATCHALL "-l([^\" ]+|\"[^\"]+\")" MPIC_LIBNAMES "${MPIC_LINK_CMDLINE}")

  # Determine full path names for all of the libraries that one needs
  # to link against in an MPI program
  set(MPIC_LIBRARIES)
  foreach(LIB ${MPIC_LIBNAMES})
    string(REGEX REPLACE "^-l" "" LIB ${LIB})
    set(MPIC_LIB "MPIC_LIB-NOTFOUND" CACHE FILEPATH "Cleared" FORCE)
    find_library(MPIC_LIB ${LIB} HINTS ${MPIC_LINK_PATH})
    if (MPIC_LIB)
      list(APPEND MPIC_LIBRARIES ${MPIC_LIB})
    else (MPIC_LIB)
      message(SEND_ERROR "Unable to find MPI library ${LIB}")
    endif (MPIC_LIB)
  endforeach(LIB)
  set(MPIC_LIB "MPIC_LIB-NOTFOUND" CACHE INTERNAL "Scratch variable for MPI detection" FORCE)

  # Chop MPIC_LIBRARIES into the old-style MPIC_LIBRARY and
  # MPIC_EXTRA_LIBRARY.
  list(LENGTH MPIC_LIBRARIES MPIC_NUMLIBS)
  list(LENGTH MPIC_LIBNAMES MPIC_NUMLIBS_EXPECTED)
  if (MPIC_NUMLIBS EQUAL MPIC_NUMLIBS_EXPECTED)
    list(GET MPIC_LIBRARIES 0 MPIC_LIBRARY_WORK)
    set(MPIC_LIBRARY ${MPIC_LIBRARY_WORK} CACHE FILEPATH "C MPI library to link against" FORCE)
  else (MPIC_NUMLIBS EQUAL MPIC_NUMLIBS_EXPECTED)
    set(MPIC_LIBRARY "MPIC_LIBRARY-NOTFOUND" CACHE FILEPATH "C MPI library to link against" FORCE)
  endif (MPIC_NUMLIBS EQUAL MPIC_NUMLIBS_EXPECTED)
  if (MPIC_NUMLIBS GREATER 1)
    set(MPIC_EXTRA_LIBRARY_WORK ${MPIC_LIBRARIES})
    list(REMOVE_AT MPIC_EXTRA_LIBRARY_WORK 0)
    set(MPIC_EXTRA_LIBRARY ${MPIC_EXTRA_LIBRARY_WORK} CACHE STRING "Extra MPI libraries to link against" FORCE)
  else (MPIC_NUMLIBS GREATER 1)
    set(MPIC_EXTRA_LIBRARY "MPIC_EXTRA_LIBRARY-NOTFOUND" CACHE STRING "Extra MPI libraries to link against" FORCE)
  endif (MPIC_NUMLIBS GREATER 1)

  # Set up all of the appropriate cache entries
  set(MPIC_COMPILE_FLAGS ${MPIC_COMPILE_FLAGS_WORK} CACHE STRING "MPI C compilation flags" FORCE)
  set(MPIC_INCLUDE_PATH ${MPIC_INCLUDE_PATH_WORK} CACHE STRING "MPI C include path" FORCE)
  set(MPIC_LINK_FLAGS ${MPIC_LINK_FLAGS_WORK} CACHE STRING "MPI C linking flags" FORCE)
else (MPIC_COMPILE_CMDLINE)
# No MPI compiler to interogate so attempt to find everything with find functions.
  find_path(MPIC_INCLUDE_PATH mpif.h
    HINTS ${_MPIC_BASE_DIR} ${_MPIC_PREFIX_PATH}
    PATH_SUFFIXES include
    )

  # Decide between 32-bit and 64-bit libraries for Microsoft's MPI
  if("${CMAKE_SIZEOF_VOID_P}" EQUAL 8)
    set(MS_MPIC_ARCH_DIR amd64)
  else()
    set(MS_MPIC_ARCH_DIR i386)
  endif()

  find_library(MPIC_LIBRARY
    NAMES mpi mpich msmpi
    HINTS ${_MPIC_BASE_DIR} ${_MPIC_PREFIX_PATH}
    PATH_SUFFIXES lib lib/${MS_MPIC_ARCH_DIR} Lib Lib/${MS_MPIC_ARCH_DIR}
    )

  find_library(MPIC_EXTRA_LIBRARY
    NAMES mpi_f77
    HINTS ${_MPIC_BASE_DIR} ${_MPIC_PREFIX_PATH}
    PATH_SUFFIXES lib
    DOC "Extra MPI libraries to link against.")

  set(MPIC_COMPILE_FLAGS "" CACHE STRING "MPI compilation flags")
  set(MPIC_LINK_FLAGS "" CACHE STRING "MPI linking flags")
endif (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)

# on BlueGene/L the MPI lib is named libmpich.rts.a, there also these additional libs are required
if("${MPIC_LIBRARY}" MATCHES "mpich.rts")
   set(MPIC_EXTRA_LIBRARY ${MPIC_EXTRA_LIBRARY} msglayer.rts devices.rts rts.rts devices.rts)
   set(MPIC_LIBRARY ${MPIC_LIBRARY}  msglayer.rts devices.rts rts.rts devices.rts)
endif("${MPIC_LIBRARY}" MATCHES "mpich.rts")

# Set up extra variables to conform to
if (MPIC_EXTRA_LIBRARY)
  set(MPIC_LIBRARIES ${MPIC_LIBRARY} ${MPIC_EXTRA_LIBRARY})
else (MPIC_EXTRA_LIBRARY)
  set(MPIC_LIBRARIES ${MPIC_LIBRARY})
endif (MPIC_EXTRA_LIBRARY)

if (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)
  set(MPIC_FOUND TRUE)
else (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)
  set(MPIC_FOUND FALSE)
endif (MPIC_INCLUDE_PATH AND MPIC_LIBRARY)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments
find_package_handle_standard_args(MPIF DEFAULT_MSG MPIC_LIBRARY MPIC_INCLUDE_PATH)

mark_as_advanced(
  MPIC_INCLUDE_PATH
  MPIC_COMPILE_FLAGS
  MPIC_LINK_FLAGS
  MPIC_LIBRARY
  MPIC_EXTRA_LIBRARY)

# unset to cleanup namespace
unset(_MPIC_PACKAGE_DIR)
unset(_MPIC_PREFIX_PATH)
unset(_MPIC_BASE_DIR)
