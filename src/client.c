/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Application
 *
 * client.c
 *
 * This is a client code to establish a connection and exchange
 * data with a remote server. After connecting to server, the client
 * can either send a number of messages to other clients, or request
 * to read its messages.
 *
 * Author: Vasileios Matsoukas, Aristotle University of Thessaloniki
 * 		   Dept. of Electrical and Computer Engineering
 * Created on: Sept 19, 2018
 *
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
#include <sys/time.h>

#define SENDER_LENGTH 3 //bear in mind the \0 or \n
#define RECEIVER_LENGTH 3
#define CONTENT_LENGTH 256


struct timeval startwtime, endwtime;
double send_time=0, receive_time=0;

void error(char *msg)
{
  perror(msg);
  exit(0);
}


//Compose and send messages to server
 void send_message(int num_msg,int sockfd)
 {
	char **temp_messages;
	char buffer[256];
	int  i, n;

	fflush(stdin);
	fflush(stdout);
	temp_messages=(char **)malloc(num_msg*sizeof(char *));
	for(i=0; i<num_msg; i++)
		temp_messages[i]=(char *)calloc(256,sizeof(char));


	//First compose messages
	for(i=0; i<num_msg; i++)
	{
		printf("Please compose message no.%d: \n",i+1);
		gets(&temp_messages[i][1]);
		temp_messages[i][0]='$';
		temp_messages[i][strlen(temp_messages[i])]='#';
		//printf("FInal Message is: %s with length %lu \n",temp_messages[i], strlen(temp_messages[i]));
	}

    //Then send them to server
	gettimeofday (&startwtime, NULL);
	for(i=0; i<num_msg; i++)
	{
		n = write(sockfd,temp_messages[i],strlen(temp_messages[i]));
		if (n < 0)
			error("ERROR writing to socket");
		//printf("%s",buffer);
	}

	//Take acknowledgement that they were sent to server
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	gettimeofday (&endwtime, NULL); //measure send time


    printf("%s",buffer);
    send_time += (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6+ endwtime.tv_sec - startwtime.tv_sec);
    printf("Total send time: %f secs\n",send_time);
    if (num_msg!=0) send_time/=num_msg;
    printf("Send time per message: %f secs\n",send_time);

 }

 //Receive client's messages from server
 void receive_messages(int sockfd)
 {
	char buffer[256];
	char tmp;
	int n,buflen;
	int i, num_msg;


	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	num_msg=atoi(buffer);
	printf("You have %d new messages in your mailbox.\n",num_msg);

	char **name, **content;

	name=(char **)malloc(num_msg * sizeof(char*));
	content=(char **)malloc(num_msg * sizeof(char*));

	for(i=0; i<num_msg;i++)
	{
		name[i]=(char *)malloc(3*sizeof(char));
		content[i]=(char *)malloc(252* sizeof(char));
	}

	for(i=0; i<num_msg; i++)
	{
		gettimeofday (&startwtime, NULL);
		for(;;)
		{
			n = read(sockfd, &tmp, 1);
			if (n<0) error("ERROR reading from socket");

			if (tmp!='$')
				  continue;

			buflen=0;
			do
			{
				n=read(sockfd,&tmp,1);
				if (n<0) error("ERROR reading from socket");

				if (tmp=='#')
						break;

				 buffer[buflen] = tmp;
				 ++buflen;
			}
			while (1);
			gettimeofday (&endwtime, NULL);
			receive_time+= (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6+ endwtime.tv_sec - startwtime.tv_sec);
			//printf("%*.*s\n", buflen, buflen, buffer);
			memcpy(name[i], buffer,2);
			memcpy(content[i], buffer+4,strlen(buffer+4));

			printf("From: %s\n", name[i]);
			printf("Content: %s\n", content[i]);
			printf("This message came back: %s\n", buffer);
			break;
		}
	}

	if(num_msg!=0)
	{

		n = write(sockfd,"Messages successfully delivered to client!\n",44);
		if (n < 0)
		   error("ERROR writing to socket");
	}

    printf("Total receive time: %f secs\n",receive_time);
    if (num_msg!=0) receive_time/=num_msg;
    printf("Receive time per message: %f secs\n",receive_time);


 }


int main(int argc, char *argv[])
{
  int sockfd, portno, n;

  struct sockaddr_in serv_addr;
  struct hostent *server;

  char buffer[256];
  char ip[INET_ADDRSTRLEN]; //THIS WAS ADDED.
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
  printf("Connected to server %s \n",ip);

  bzero(buffer,256);

  printf("Welcome to message exchange app.\n");
  printf("Type 'send(numberofmessages)' to send messages (e.g. send10) or 'read(clientname)' to read your messages (e.g. readnp): ");
  printf("\n");
  char choice[10];
  for(;;)
  {
	  fgets(choice,10,stdin);
	  if (strncmp(choice,"send\0",4)==0)
	  {
		  memcpy(buffer,choice,strlen(choice));
		  n=write(sockfd,buffer,strlen(buffer)); //send the num of messages that will be sent.
		  if (n<0) error("ERROR writing to socket");
		  int num=atoi(&choice[4]);
		  send_message(num,sockfd);
		  break;
	  }
	  else if (strncmp(choice,"read\0",4)==0)
	  {
		  memcpy(buffer,choice,strlen(choice));
		  n=write(sockfd,buffer,strlen(buffer)); //send the name of client to retrieve messages.
		  if (n<0) error("ERROR writing to socket");

		  receive_messages(sockfd);
		  break;

	  }
	  else
	  {
		  printf("Wrong input.\n");
		  printf("Type 'send' to send messages or 'read' to read your messages: ");
		  printf("\n");

	  }

  }

  return 0;
}
