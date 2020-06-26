#!/bin/bash

if [ $# -lt 1 ]; then
  echo "usage: batchScanMUSIC fulpathdatadir"
  exit 1
fi

WORKINGDIRECTORY=/lustre/nyx/alice/users/vgonzale/UrQMDparser/SANDBOX
DATADIRROOT=$1
NEVENTS=1000000
PRODUCTIONDIRECTORY=OUT`date +%Y%m%d%H%M`

if [ -d $WORKINGDIRECTORY/$PRODUCTIONDIRECTORY ]
then
  echo Production directory $PRODUCTIONDIRECTORY does exist. WAIT AT LEAST ONE MINUTE!!!!
  exit 1	
fi

mkdir $WORKINGDIRECTORY/$PRODUCTIONDIRECTORY
echo Production $PRODUCTIONDIRECTORY

for CENTRALITY in 0-10 10-20 20-30 30-40 40-50 50-60 60-70 70-80 80-90 90-100
do
  # launching the sub-jobs for the whole set of centralities
  DATADIRECTORY=$1/RESULTS$CENTRALITY
  mkdir $DATADIRECTORY/$PRODUCTIONDIRECTORY

  sbatch -J batch__scanMUSIC --array=1-100 --workdir=$WORKINGDIRECTORY --mem-per-cpu=8000 --time=02:00:00 -o $DATADIRECTORY/$PRODUCTIONDIRECTORY/Job_%A_%a.out -e $DATADIRECTORY/$PRODUCTIONDIRECTORY/Job_%A_%a.err /lustre/nyx/alice/users/vgonzale/UrQMDparser/SANDBOX/scanMUSIC.sh $DATADIRECTORY $PRODUCTIONDIRECTORY $CENTRALITY $NEVENTS

  sleep 4s
done

