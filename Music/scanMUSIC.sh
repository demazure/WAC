#!/bin/bash

DATADIRECTORY=$1
PRODUCTION=$2
CENTNAME=$3
NEVENTS=$4

DATAFILEIX=$SLURM_ARRAY_TASK_ID
DATAFILE=$DATADIRECTORY/UrQMD_output_$DATAFILEIX.root

if [ -f $DATAFILE ]
then

  # Execute application code
  source /etc/profile.d/modules.sh
  module use /cvmfs/it.gsi.de/modulefiles/
  module load root/v6.06-06
  export ROOTSYS=/cvmfs/it.gsi.de/root/v6-06-06

  root -q -l -b "runMUSIC.c(\"$DATAFILE\",\"$CENTNAME\",\"$DATADIRECTORY/\",\"$PRODUCTION/\",$DATAFILEIX,$NEVENTS)";
fi


