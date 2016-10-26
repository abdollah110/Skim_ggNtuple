#!/bin/bash
PROCESS=0
num=3
lines2=($(cat ListName.txt)) # array
echo ${lines2[$PROCESS]}
input2=${lines2[$PROCESS]}
make
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"$input2 | grep $num.root
xrdfs root://cmseos.fnal.gov ls "/eos/uscms/store/user/abdollah/Crab3/ggNtuple/V1/"$input2 | grep $num.root | while read line2
do
 
 file=`echo $line2`
 Name=${file##*V1}
 echo "Here is the first check   ------>" $Name
# ./Skimmer  $Name
done  
IFS="/"
set $input2
OutName=$4".root"
echo $OutName
#hadd -f $OutName "skimed_"*.root
#xrdcp $OutName root://cmseos.fnal.gov//store/user/abdollah/Crab3/Hadd/$OutName
echo "Done execution ..."


