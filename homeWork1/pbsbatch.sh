#!/bin/sh
### Set the job name (for your reference)
#PBS -N FPTree_aaditya
### Set the project name, your department code by default
#PBS -P CSE
### Request email when job begins and ends
#PBS -m bea
### Specify email address to use for notification.
#PBS -M $USER@iitd.ac.in
####
#PBS -l select=1:ncpus=1
### Specify "wallclock time" required for this job, hhh:mm:ss
#PBS -l walltime=01:00:00

# After job starts, must goto working directory. 
# $PBS_O_WORKDIR is the directory from where the job is fired. 
echo "==============================="
echo $PBS_JOBID
cat $PBS_NODEFILE
echo "==============================="
cd $PBS_O_WORKDIR
#job 
./$1 ./$2 $3 ./$4
#NOTE
# The job line is an example : users need to change it to suit their applications
# The PBS select statement picks n nodes each having m free processors
# OpenMPI needs more options such as $PBS_NODEFILE
