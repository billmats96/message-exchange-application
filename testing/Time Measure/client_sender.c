/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Applications
 *
 * client_sender.c
 *
 * A specific version of client.c used for sending messages and
 * measuring send time.
 *
 * Author: Vasileios Matsoukas, Aristotle University of Thessaloniki
 * 		   Dept. of Electrical and Computer Engineering
 * Created on: Sept 19, 2018
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
struct timeval start, end;
double send_time=0, total_time=0, receive_time=0;


void error(char *msg)
{
  perror(msg);
  exit(0);
}


 void send_message(int num_msg,int sockfd, char *message)
 {
	char **temp_messages;
	char buffer[256];
	int  i, n;

	temp_messages=(char **)malloc(num_msg*sizeof(char *));
	for(i=0; i<num_msg; i++)
		temp_messages[i]=(char *)calloc(256,sizeof(char));

	for(i=0; i<num_msg; i++)
	{
		strncpy(&temp_messages[i][1],message,strlen(message));
		temp_messages[i][0]='$';
		temp_messages[i][strlen(temp_messages[i])]='#';
	}

    //Send Messages to server
	gettimeofday (&startwtime, NULL);
	for(i=0; i<num_msg; i++)
	{
		n = write(sockfd,temp_messages[i],256);
		if (n < 0)
			error("ERROR writing to socket");
	}

	//Take acknowledgement that they were sent to server
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	gettimeofday (&endwtime, NULL);


	//Send time
    send_time += (double)((endwtime.tv_usec - startwtime.tv_usec)/1.0e6+ endwtime.tv_sec - startwtime.tv_sec);
   // printf("Total send time: %f secs\n",send_time);
    if (num_msg!=0) send_time/=num_msg;
    //printf("Send time per message: %f secs\n",send_time);
    printf("Send time (per msg): %f\n",send_time);



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
  gettimeofday(&start,NULL);
  bzero(buffer,256);
  char *temp=(char *)malloc(10*sizeof(char));

  memcpy(temp,argv[4]+4,strlen(argv[4]+4));
  int num_msg=atoi(temp);
  n=write(sockfd,argv[4],strlen(argv[4])); //send the num of messages that will be sent.
  send_message(num_msg,sockfd,argv[3]);
  free(temp);
  gettimeofday(&end,NULL);
  total_time += (double)((end.tv_usec - start.tv_usec)/1.0e6+ end.tv_sec - start.tv_sec);
  printf("Total time: %f\n",total_time);
  printf("(Service Time/ Overall Usage) percentage = %f \n",send_time*num_msg/total_time*100);
  return 0;
}


