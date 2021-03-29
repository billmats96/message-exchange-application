#!/bin/bash
#Bash script to simulate big client load  
#Now every client sends more than 1 message and we try to give an estimation of the send and receive times

clear

echo Running super script..

CALM_TIME="10"
HOSTNAME="192.168.1.1" 
#HOSTNAME="127.0.0.1"
PORT="2223"
READ="readaa"



#-------------------------------------------#
CLIENTS="1"   		#Number of clients in the system
EMIT_TIMES="5" 		#How many independent times the client will log in to send messages  
#SEND="send10" 		#Messages per independent try (and per client)
EMISSION_PERIOD="0.18"  #Time between two independent tries (frequency of log in to send msg)

echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.02 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME #let the server calm down and go to next experiment
 

#-------------------------------------------#
CLIENTS="2"
EMIT_TIMES="5"
EMISSION_PERIOD="0.4" 

echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
    (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.02 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME #let the server calm down and go to next experiment



#-------------------------------------------#
CLIENTS="10"
EMIT_TIMES="4"
EMISSION_PERIOD="1.5"

echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
    (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.1 #time betweeen clients creation
done 


echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME #let the server calm down and go to next experiment



#-------------------------------------------#
CLIENTS="20"
EMIT_TIMES="4"
EMISSION_PERIOD="3"
echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.2 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME  #let the server calm down and go to next experiment



#-------------------------------------------#
CLIENTS="50"
EMIT_TIMES="3"
EMISSION_PERIOD="3"
echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.2 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME  #let the server calm down and go to next experiment


#-------------------------------------------#
CLIENTS="100"
EMIT_TIMES="2"
EMISSION_PERIOD="4"
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.25 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME  #let the server calm down and go to next experiment


#-------------------------------------------#
CLIENTS="500"
EMIT_TIMES="1"
EMISSION_PERIOD="5"

echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.25 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME  #let the server calm down and go to next experiment


#-------------------------------------------#
CLIENTS="700"
EMIT_TIMES="1"
EMISSION_PERIOD="5"

echo ---------Results with clients = $CLIENTS ------------ &>>f.txt 
echo Clients: $CLIENTS connecting every $EMISSION_PERIOD secs to send a message 

for i in $(seq 1 $CLIENTS) 
do 
     (./client_message_attack2.sh $EMIT_TIMES $EMISSION_PERIOD &)    
sleep 0.25 #time betweeen clients creation
done 

echo Receive time: &>>f.txt
./client_receiver $HOSTNAME $PORT $READ &>>f.txt

sleep $CALM_TIME  #let the server calm down and go to next experiment



#sleep 5 #sleep for a time and then kill them

#pgrep client | xargs kill -9

echo End of super script..
