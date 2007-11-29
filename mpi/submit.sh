#!/bin/bash

#PBS -N reg_mpi_sim
#PBS -l nodes=1:ppn=2
#PBS -l vmem=2G
#PBS -l walltime=0:20:00
#PBS -o /nfs/users/ngs0011/work/RealityGrid/file/reg_steering_school/mpi/mpi_sim.out
#PBS -e /nfs/users/ngs0011/work/RealityGrid/file/reg_steering_school/mpi/mpi_sim.err
#PBS -V

. /nfs/users/ngs0011/work/RealityGrid/file/reg_steer_lib/reg_steer_configure.sh
export REG_STEER_DIRECTORY=/nfs/users/ngs0011/tmp
env | grep REG

mpiexec -np 2 /nfs/users/ngs0011/work/RealityGrid/file/reg_steering_school/mpi/mpi_simulation
