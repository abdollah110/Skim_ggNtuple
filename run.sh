#!/bin/bash

echo "input parameters: cluster, process, run path, out path, sample name" $1 $2 $3 $4 $5
CLUSTER=$1
PROCESS=$2
RUNPATH=$3
OUTPATH=$4
SAMPLE=$5

echo ""
echo "CMSSW on Condor"
echo ""

START_TIME=`/bin/date`
echo "started at $START_TIME"

echo ""
echo "parameter set:"
echo "CLUSTER: $CLUSTER"
echo "PROCESS: $PROCESS"
echo "RUNPATH: $RUNPATH"
echo "OUTPATH: $OUTPATH"
echo "SAMPLE: $SAMPLE"

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo $PWD
cd /uscmst1b_scratch/lpc1/old_scratch/lpceg/yurii/lovedeep/ggNtuples/CMSSW_7_2_2_patch1/src/
eval `scram runtime -sh`
echo $PWD , "for cmsenv"
echo ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
echo $PWD , "for job running"
let "var++"
let "count=${2}+1"
cp $RUNPATH/Skimmer.cc .
cp $RUNPATH/Skimmer.h .
cp $RUNPATH/Makefile .
cp $RUNPATH/Sample13TeV.txt .



old_IFS=$IFS
IFS=$'\n'
lines=($(cat Sample13TeV.txt)) # array
IFS=$old_IFS
echo ${lines[$PROCESS / 20]}
input=${lines[$PROCESS / 20]}
FracNum=$(($PROCESS % 20))
arrIN=(${input// / })
echo "split number is",  $FracNum
echo "input file=", $input
make
./Skimmer  $input $FracNum
#./Skimmer /uscms_data/d3/abdollah/Analysis/LQ2016/Skim_lxplus/ForCopy/M900.root  $PROCESS
echo "Done execution ..."
END_TIME=`/bin/date`
echo "finished at ${END_TIME}"
exit $exitcode
echo "DONE!"

