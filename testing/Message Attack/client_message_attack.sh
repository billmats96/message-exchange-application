#!/bin/bash
#Bash script to simulate big client load  

#clear

echo Running sub script..

HOSTNAME="192.168.1.1" 
#HOSTNAME="127.0.0.1"
PORT="2223"
DATA="aaaaatackkkkkk!"
NUM_MESSAGES="1"
SEND="send1"
EMIT_TIMES=$1
EMISSION_PERIOD=$2 


for i in $(seq 1 $1)
do 
     ( ./client_sender $HOSTNAME $PORT $DATA $SEND>f.txt &)  
sleep $2
done 

sleep 3 #sleep for a time and then kill them

pgrep client_sender | xargs kill -9

echo End of sub script..
