#!/bin/bash
Address="/store/user/abdollah/Crab3/ggNtuple/V1/MC/"
xrdfs $Address  | while read line
do
 echo $line
 file = `echo  $line ` 

done
 
  
