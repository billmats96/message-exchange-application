#!/bin/bash
#Bash script to simulate big client load  

clear

echo Running super script..

HOSTNAME="192.168.1.1" 
#HOSTNAME="127.0.0.1"
CALM_TIME="10"
PORT="2223"
READ="readaa"

#-------------------------------------------#
CLIENTS="1"
EMIT_TIMES="150"
EMISSION_PERIOD="0.12"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.02 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME #let the server calm down and go to next experiment



#-------------------------------------------#
CLIENTS="2"
EMIT_TIMES="50"
EMISSION_PERIOD="0.4"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
    (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.04 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME #let the server calm down and go to next experiment


#-------------------------------------------#
CLIENTS="10"
EMIT_TIMES="20"
EMISSION_PERIOD="1.4"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
    (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.08 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME #let the server calm down and go to next experiment


#-------------------------------------------#
CLIENTS="20"
EMIT_TIMES="5"
EMISSION_PERIOD="2.4"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.12 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME  #let the server calm down and go to next experiment


CLIENTS="50"
EMIT_TIMES="2"
EMISSION_PERIOD="3"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.16 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME  #let the server calm down and go to next experiment


CLIENTS="100"
EMIT_TIMES="1"
EMISSION_PERIOD="4"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.2 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME  #let the server calm down and go to next experiment


CLIENTS="500"
EMIT_TIMES="1"
EMISSION_PERIOD="5"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.25 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME  #let the server calm down and go to next experiment

CLIENTS="700"
EMIT_TIMES="1"
EMISSION_PERIOD="5"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.25 #time betweeen clients creation
done 

./client_receiver $HOSTNAME $PORT $READ >/dev/null

sleep $CALM_TIME  #let the server calm down and go to next experiment



#sleep 5 #sleep for a time and then kill them

#pgrep client | xargs kill -9

echo End of super script..
