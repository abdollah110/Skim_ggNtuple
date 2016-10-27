#!/bin/bash
#
#echo "input parameters: cluster, process, run path, out path, sample name" $1 $2 $3 $4
#CLUSTER=$1
#PROCESS=$2
#RUNPATH=$3
#OUTPATH=$4


#########For running locally uncommnet this line
CLUSTER=""
PROCESS=5
RUNPATH="/uscms_data/d3/abdollah/Analysis/LQ2016/CMSSW_8_0_11/src/Skim_ggNtuple"
OUTPATH=""


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

source /cvmfs/cms.cern.ch/cmsset_default.sh
echo $PWD
cd $RUNPATH
eval `scram runtime -sh`
echo $PWD , "for cmsenv"
ls 
echo "check whether eos is working here"
echo ${_CONDOR_SCRATCH_DIR}
cd ${_CONDOR_SCRATCH_DIR}
echo $PWD , "for job running"


###### Copy the neccessary files for running the Skim
cp $RUNPATH/Skimmer.cc .
cp $RUNPATH/Skimmer.h .
cp $RUNPATH/Makefile .
cp $RUNPATH/ListName.txt .





#########  Smaple/Job splitting
SplitingNumber=10
DataSetArray=($(cat ListName.txt)) # array of the input datadets
echo ${DataSetArray[$PROCESS / $SplitingNumber]}
DataSetName=${DataSetArray[$PROCESS / $SplitingNumber]}
rootNumber=$(($PROCESS % $SplitingNumber))


########### complie the Skimmer
make


########### loop over all root file in a dataset directory
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/80X/"$DataSetName | grep $rootNumber.root | while read FullDataSetName

############  Here is where the Skimmer is running     ############
do
 file=`echo $FullDataSetName`
 ShortName=${file##*80X}  # This removes all the string before 80X (including 80X)
 echo "Here is the short Name   ------>" $ShortName
 ./Skimmer  $ShortName
done
############  Here is where the Skimmer ends          ############



IFS="/"
set $DataSetName
OutName=$4$6$rootNumber".root"
echo $OutName
hadd -f $OutName "skimed_"*.root




##########  remove the unneccesat files
rm skim*root  Skimmer.cc  Skimmer.h  Makefile  ListName.txt
echo "Done execution ..."




END_TIME=`/bin/date`
echo "finished at ${END_TIME}"

exit $exitcode
echo "DONE!"

