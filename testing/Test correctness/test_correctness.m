%---------------test_correctness.m Script-----------------
%  Script to test client-server code correctness .
% This scripts generates random strings (of length>=6) that represent
% random messages. There is a specif executable for sending the random
% message (client_sender) and a specific executable (client_receiver)  for receiving the
% message that was intended for the random client (which is determined by the
% random message). We execute client_sender to send the random message and
% then execute client_receiver to receive the message. If the message
% received is same with the message sent, then this is a successful try. We
% verify code correctness by executing this routine many times. If the
% success percentage is 100% over all these tries, then we can maintain
% that the code works right.
%
% Make sure that the server is up and runnig before you run the test. The
% server code can either run on PC or the ZSun. The server code has to be
% set so it can host the desired number of clients(#define MAX_CLIENTS>=
% tries). Also, compile the source codes, in case the executables do not
% exist. Check that the parametres such as host_address and port are
% valid.
 


 %% First run a makefile or compile the source codes and start server. 
 
 %% Define test parametres
 tries=10;  %Also make sure that you jave define MAX_CLIENTS>=tries in server code
 success_tries=0; %Count successful tries
 
 symbols = ['a':'z' ]; % could also be symbols = ['a':'z' 'A':'Z' '0':'9']; but is chosen so for simplicity
 MAX_ST_LENGTH = 10; 
 
 for i=1:tries
    stLength = randi(MAX_ST_LENGTH)+6; %Ensure that length is at least 6, so that it results a valid message.
    nums = randi(numel(symbols),[1 stLength]);
    st = symbols (nums); %random message

    %Execute a send 
     executable_file="client_sender";
     host_address="127.0.0.1";  %Change it accordingly
     port="2223" ;  %Must be the same with server's port
     message_to_send=st;
 
    command=['./',char(executable_file),' ',char(host_address),' ',char(port),' ',char(message_to_send)];
    [status1,cmdout1] = system(command);
    cmdout1 

    %Execute a receive
    executable_file="client_sender";
    message_to_read="read"+st(3:4); %st(3:4) is the target client, the messages of whom we want to read

    command=['./',char(executable_file),' ',char(host_address),' ',char(port),' ',char(message_to_send)];
    [status2,cmdout2] = system(command);
     cmdout2   

    %Verify that the random message was successfully sent and received by
    %the random client
    if cmdout1==cmdout2
        fprintf("Success.\n");
        success_tries=success_tries+1;
    else
         printf("Fail");
    end
    
 end
 
 success_percentage=success_tries/tries*100;
 
 fprintf("Success percentage is %f \n ",success_percentage);
 
 
%%-------------------------------------------------------------------------
%
% AUTHOR
%
%   Matsoukas Vasileios,
%   Undergraduate Student, Department of Electrical and Computer Engineering 
%   Aristotle University of Thessaloniki, Greece
%   AEM:8743
%   email: vmatsouk@auth.gr
%
% -------------------------------------------------------------------------