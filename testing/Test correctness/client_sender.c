/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Applications
 *
 * client_sender.c
 *
 * A specific version of client.c used for testing correctness.
 * Sends one message to server
 *
 * Author: Vasileios Matsoukas, Aristotle University of Thessaloniki
 * 		   Dept. of Electrical and Computer Engineering
 * Created on: Apr 19, 2018
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define SENDER_LENGTH 3 //bear in mind the \0 or \n
#define RECEIVER_LENGTH 3
#define CONTENT_LENGTH 256


void error(char *msg)
{
  perror(msg);
  exit(0);
}


 void send_message(int num_msg,int sockfd, char *message)
 {
	struct message msg;
	char **temp_messages;
	char num_str[3], buffer[256];
	int  i, n;

	sleep(1);
	temp_messages=(char **)malloc(num_msg*sizeof(char *));
	for(i=0; i<num_msg; i++)
		temp_messages[i]=(char *)calloc(256,sizeof(char));

	for(i=0; i<num_msg; i++)
	{
		strncpy(&temp_messages[i][1],message,strlen(message));
		printf("%s",message);
		temp_messages[i][0]='$';
		temp_messages[i][strlen(temp_messages[i])]='#';
	}

    //Send messagges to server
	for(i=0; i<num_msg; i++)
	{
		n = write(sockfd,temp_messages[i],256);
		if (n < 0)
			error("ERROR writing to socket");
	}

	//Take acknowledgement that they were sent to server
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
   //printf("%s",buffer);

 }


int main(int argc, char *argv[])
{
  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];
  char ip[INET_ADDRSTRLEN];
  if (argc < 3) {
    fprintf(stderr,"usage %s hostname port\n", argv[0]);
    exit(0);
  }
  portno = atoi(argv[2]);
  sockfd = socket(AF_INET, SOCK_STREAM, 0);

  if (sockfd < 0)
    error("ERROR opening socket");

  server = gethostbyname(argv[1]);
  if (server == NULL)
  {
    fprintf(stderr,"ERROR, no such host\n");
    exit(0);
  }
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
  serv_addr.sin_port = htons(portno);
  if (connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR connecting");
  inet_ntop(AF_INET, (struct sockaddr *)&serv_addr, ip, INET_ADDRSTRLEN);
  bzero(buffer,256);


  n=write(sockfd,"send1.",strlen("send1")); //send the num of messages that will be sent.
  send_message(1,sockfd,argv[3]);

  return 0;
}


