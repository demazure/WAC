#!/bin/bash

if [ $# -gt 2 ]; then
  echo "usage: batchRunPythiaCorrelations basedirectory nmainjobs"
  exit 1
fi

if [ $# -lt 2 ]; then
  echo "usage: batchRunPythiaCorrelations basedirectory nmainjobs"
  exit 1
fi

BASEDIRECTORY=$1
NMAINJOBS=$2
PRODUCTIONDIRECTORY=OUT`date +%Y%m%d%H%M`

if [ -d $BASEDIRECTORY/$PRODUCTIONDIRECTORY ]
then
  echo Production directory $PRODUCTIONDIRECTORY does exist. WAIT AT LEAST ONE MINUTE!!!!
  exit 1	
fi

mkdir $BASEDIRECTORY/$PRODUCTIONDIRECTORY
echo Production $PRODUCTIONDIRECTORY

for ijob in $(seq 1 $NMAINJOBS)
do
  # launching the main generation jobs
  WORKINGDIRECTORY=$BASEDIRECTORY/$PRODUCTIONDIRECTORY/$(printf "BUNCH%02d" $ijob)
  mkdir -p $WORKINGDIRECTORY/Output

  sbatch -J batch__PythiaCorr --array=1 --chdir=$WORKINGDIRECTORY --time=03:00:00 -o $WORKINGDIRECTORY/Job_%A_%a.out -e $WORKINGDIRECTORY/Job_%A_%a.err /home/victorG/PROJECTS/WAC/Clusters/UCM/runPythiaCorrelations.sh

  sleep 2s
done

