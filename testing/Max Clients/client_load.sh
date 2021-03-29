#!/bin/bash
#Bash script to simulate big client load  

clear

echo Running script..

HOSTNAME="192.168.1.1"
PORT="2223"

#server has to be up and running in the embedded device (Zsun)

for i in {1..2000}
do 
     ( ./client $HOSTNAME $PORT & ) 
sleep 0.03
done 

sleep 5 #sleep for a time and then kill them

pgrep client | xargs kill -9

echo End of script..
