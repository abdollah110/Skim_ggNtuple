#!/bin/bash


echo ""
echo "CMSSW on Condor"
echo ""

START_TIME=`/bin/date`
echo "started at $START_TIME"


source /cvmfs/cms.cern.ch/cmsset_default.sh
eval `scram runtime -sh`
echo $PWD , "for cmsenv"
ls 
echo "check whether eos is working here"
eos root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"
echo $PWD , "for job running"

PROCESS=3

lines2=($(cat ListName.txt)) # array
echo ${lines2[$PROCESS / 10]}
input2=${lines2[$PROCESS / 10]}
NumberCrab=$(($PROCESS % 10))


make
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"$input2 | grep $NumberCrab.root
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"$input2 | grep $NumberCrab.root | while read line2
do
 
 file=`echo $line2`
 Name=${file##*V1}
 echo "Here is the first check   ------>" $Name
 ./Skimmer  $Name
done  
IFS="/"
set $input2
OutName=$4$6$NumberCrab".root"
echo $OutName
hadd -f $OutName "skimed_"*.root
#xrdcp $OutName root://cmseos.fnal.gov//store/user/abdollah/Crab3/Hadd/$OutName
echo "Done execution ..."




END_TIME=`/bin/date`
echo "finished at ${END_TIME}"

exit $exitcode
echo "DONE!"

