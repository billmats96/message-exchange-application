/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Applications
 *
 * client_receiver.c
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
#include <sys/time.h>

#define SENDER_LENGTH 3 //bear in mind the \0 or \n
#define RECEIVER_LENGTH 3
#define CONTENT_LENGTH 256

struct timeval startwtime, endwtime;
double send_time=0,total_time=0, receive_time=0;
struct timeval start, end;
int number_of_messages;

void error(char *msg)
{
  perror(msg);
  exit(0);
}


 void receive_messages(int sockfd)
 {
	char buffer[256];
	char tmp;
	int n,buflen;
	int i, num_msg;

	bzero(buffer,256);
	n = read(sockfd,buffer,255);
	num_msg=atoi(buffer);
//	printf("You have %d new messages in your mailbox.\n",num_msg);
	number_of_messages=num_msg;
	char **name, **content;
	struct message** msg =(struct message **)malloc(num_msg*sizeof(struct message *));

	name=(char **)malloc(num_msg * sizeof(char*));
	content=(char **)malloc(num_msg * sizeof(char*));

	for(i=0; i<num_msg;i++)
	{
		name[i]=(char *)malloc(3*sizeof(char));
		content[i]=(char *)malloc(252* sizeof(char));
	}

	for(i=0; i<num_msg; i++)
	{
		for(;;)
		{
			gettimeofday (&startwtime, NULL);
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
			memcpy(name[i], buffer,2);
			memcpy(content[i], buffer+4,strlen(buffer+4));
			break;
		}
	}

	n = write(sockfd,"Messages successfully delivered to client!\n",256);
	if (n < 0)
	   error("ERROR writing to socket");

    //printf("Total receive time: %f secs\n",receive_time);
    if (num_msg!=0) receive_time/=num_msg;
   // printf("Receive time per message: %f secs\n",receive_time);
    printf("%f\n",receive_time);


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
  gettimeofday(&start,NULL);
  bzero(buffer,256);

  n=write(sockfd,argv[3],strlen(argv[3])); //send the num of messages that will be sent.
  if (n<0) error("ERROR writing to socket");
  receive_messages(sockfd);
  gettimeofday(&end,NULL);
  total_time += (double)((end.tv_usec - start.tv_usec)/1.0e6+ end.tv_sec - start.tv_sec);
  printf("Total time: %f\n",total_time);
  printf("(Service Time/ Overall Usage) percentage = %f \n",receive_time*number_of_messages/total_time*100);

  return 0;
}

