#!/bin/bash
#Bash script to simulate big client load 
#Now every client sends more than 1 message and we try to give an estimation of the send and receive times

#clear

echo Running sub script..

HOSTNAME="192.168.1.1" 
#HOSTNAME="127.0.0.1"
PORT="2223"
DATA="aaaatestmessage" #15 chars (+2 =17 due to delimiters that are added)
READ="readaa"
SEND="send5"
EMIT_TIMES=$1
EMISSION_PERIOD=$2 


for i in $(seq 1 $1)
do 
#if [ $i%2==1 ] 
#then    
#    ( ./client_sender $HOSTNAME $PORT $DATA $SEND&>>f.txt &)  
#else    
#    ( ./client_receiver $HOSTNAME $PORT $READ&>>f.txt &)  
#fi
( ./client_sender $HOSTNAME $PORT $DATA $SEND&>>f.txt &)  
sleep $2
done 

#sleep 5 #sleep for a time and then kill them

#pgrep client | xargs kill -9

#echo End of script..
