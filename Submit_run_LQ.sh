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
cd $RUNPATH
eval `scram runtime -sh`
echo $PWD , "for cmsenv"
ls 
echo "check whether eos is working here"
eos root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/80X/"
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/80X/"
echo ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
echo $PWD , "for job running"
let "var++"
let "count=${2}+1"
cp $RUNPATH/Skimmer.cc .
cp $RUNPATH/Skimmer.h .
cp $RUNPATH/Makefile .
cp $RUNPATH/ListName.txt .


NumberOfJeob=10
lines2=($(cat ListName.txt)) # array
echo ${lines2[$PROCESS / $NumberOfJeob]}
input2=${lines2[$PROCESS / $NumberOfJeob]}
NumberCrab=$(($PROCESS % $NumberOfJeob))


make
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/80X/"$input2 | grep $NumberCrab.root
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/80X/"$input2 | grep $NumberCrab.root | while read line2
do
 
 file=`echo $line2`
 Name=${file##*80X}
 echo "Here is the first check   ------>" $Name
 ./Skimmer  $Name
done  
IFS="/"
set $input2
OutName=$4$6$NumberCrab".root"
echo $OutName
hadd -f $OutName "skimed_"*.root
#xrdcp $OutName root://cmseos.fnal.gov//store/user/abdollah/Crab3/Hadd80X/$OutName
#touch $OutName.txt
rm skimed*.root  Skimmer.cc  Skimmer.h  Makefile  ListName.txt
echo "Done execution ..."




END_TIME=`/bin/date`
echo "finished at ${END_TIME}"

exit $exitcode
echo "DONE!"

