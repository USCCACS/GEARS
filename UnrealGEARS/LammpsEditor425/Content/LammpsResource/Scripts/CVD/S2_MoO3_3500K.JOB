#!/bin/sh
#PBS -l nodes=1:ppn=1,walltime=24:00:00
#PBS -n 2000K_S_MoO3
#PBS -q priya
#PBS -A lc_pv
#PBS -m ae -M  sungwooh@usc.edu
#PBS -o log
#PBS -d . 
#PBS -j oe

source /usr/usc/intel/default/setup.sh
source /usr/usc/openmpi/default/setup.sh.intel

mpirun -np 1 /home/rcf-40/sungwooh/work/software/lammps-17Nov16/src/lmp_mpi < in.ANP > out
