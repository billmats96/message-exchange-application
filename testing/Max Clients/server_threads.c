/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Application
 *
 * server_threads.c
 * 
 *
 * This server code is enhanced so it can handle simultaneous connections,
 * that is multiple clients at a time. After a connection with a client is established,
 * a new thread is created to handle the communication. The server can either receive and save
 * messages from clients, or deliver messages that are intended for them. A special structure is
 * used to handle message flow. A hash table that keeps the names of the clients and a linked list
 * associated to each client, representing client's mailbox. When the communication is over,
 * the thread returns. The server is running infinitely and awaits client requests.
 *
 * Author: Vasileios Matsoukas, Aristotle University of Thessaloniki
 * 		   Dept. of Electrical and Computer Engineering
 * Created on: Sept 19, 2018
 *
 */
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>

#define MAX_CLIENTS 700 //Set the number of desired clients

#define MAX_UNQUEUED_CLIENTS 500

#include "hash_list.h"
#include "queue.h"


pthread_mutex_t lock;
int thread_count=0;

struct queue *q;



void error(char *msg)
{
  perror(msg);
  exit(1);
}

//Save the messages that client sends to server
void receive_messages(int num_msg,int sockfd)
{
	printf("---------Receive messages---------\n");
	char buffer[256];
	char tmp;
	int buflen,n,i;

	printf("Number of incoming messages is: %d \n",num_msg);

	char **client_name=(char **)malloc(num_msg * sizeof(char*));
	char **message=(char **)malloc(num_msg * sizeof(char*));

	for(i=0; i<num_msg;i++)
	{
		client_name[i]=(char *)malloc(3*sizeof(char));
		message[i]=(char *)malloc(256* sizeof(char));
	}

	printf("Clients' state on system: \n");
	display_clients();

	bzero(buffer,256);
	for(i=0; i<num_msg; i++)
	{

		for(;;)
		{
			n=read(sockfd,&tmp,1);
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
			}while (1);

			memcpy(client_name[i], buffer+2, 2);
			memcpy(message[i],buffer,strlen(buffer));

			pthread_mutex_lock(&lock);
			client=search_client(client_name[i]);
			if(client!=NULL)
			{
				printf("Client found!\n");
			}
			else
			{
				printf("Client not found. Registering client %s \n",client_name[i]);
				register_client(client_name[i]);
			}
			client=search_client(client_name[i]);

			printf("This message: %s is added to client %s\n", message[i], client_name[i]);
			client->message=add_message(client->message, message[i]);
			display_clients();
			pthread_mutex_unlock(&lock);

			bzero(buffer,256);
			break;
		}
	}

	//Send back an acknowledgement
	n = write(sockfd,"Messages successfully delivered to server!\n",44);
	if (n < 0)
	   error("ERROR writing to socket");
}

//Send messages saved in server back to the client
void deliver_messages(char *client_name, int sockfd)
{
	printf("---------Deliver messages---------\n");
	char buffer[256];
	int n, num_msg;
	struct Client* client_temp=(struct Client *)malloc(sizeof(struct Client));


	pthread_mutex_lock(&lock); //
	client_temp=search_client(client_name);
	if(client_temp!=NULL)
	{
		printf("Client found. Retreiving messages of %s \n",client_name);
	}
	else
	{
		 pthread_mutex_unlock(&lock);
		bzero(buffer,256);
		n=write(sockfd,buffer,256);
		printf("Client %s does not exist.\n",client_name);
		return; //There is no such client. Return to prevent segmentation fault.
	}
	display_clients();

		print_message_list(client_temp->message);

		num_msg=list_length(client_temp->message);
		sprintf(buffer,"%d",num_msg);
		n=write(sockfd,buffer,256);
		if (n<0)
			error("ERROR writing to socket");


		bzero(buffer,256);
	    NODE * temp;
	    for (temp=client_temp->message; temp; temp=temp->next)
	    {

	        //printf("[ %s ]", temp->data);
	        buffer[0]='$';
	        memcpy(buffer+1,temp->data,strlen(temp->data));
	        buffer[strlen(buffer)]='#';
	        printf("This message will be sent back: %s\n", buffer);
	        n = write(sockfd,buffer,strlen(buffer));
	        if (n < 0) error("ERROR writing to socket");
	        bzero(buffer,256);

	    }
	    printf("\n");

	    //Free client's messages.
	    client_temp->message=free_message_list(client_temp->message);
	    pthread_mutex_unlock(&lock);

		//Take acknowledgement that they were sent to client
		bzero(buffer,256);
		n = read(sockfd,buffer,44);
	    printf("%s",buffer);

}


//Thread for client handling
void *client_handler(void *sock)
{

	pthread_mutex_lock(&lock);
	thread_count+=1;
	pthread_mutex_unlock(&lock);

	int tid=thread_count;

	printf("Total clients currently on system: %d\n", tid); //this will be removed

	char buffer[256];
    char *temp=(char *)malloc(10*sizeof(char));
    char *name=(char *)malloc(3*sizeof(char));
    int n;
    int newsockfd=*(int *)sock;

	bzero(buffer,256);
	n = read(newsockfd,buffer,256); //5 -> change to.....10? 11? . one more bug is that if client 4 sends "Send1" faster than client3 "it steals priority.
	memcpy(temp,buffer,strlen(buffer));


	if(tid>=MAX_UNQUEUED_CLIENTS)
	{
		pthread_mutex_lock(&lock);
		enq(q,tid);
		pthread_mutex_unlock(&lock);
	}
	if(tid>=MAX_UNQUEUED_CLIENTS)
	{
		for(;;)
		{
			if(thread_count>MAX_UNQUEUED_CLIENTS)
			{

				usleep(10000);
			}
			else
			{
				if(peek(q)==tid)
				{
					pthread_mutex_lock(&lock);
					int c=deq(q);
					pthread_mutex_unlock(&lock);
					break;
				}
				else
				{
					usleep(10000);
				}
			}
		}
	}



	//Case that the client sends messages
	if (strncmp(temp,"send\0,",4)==0)
	{

		//get the number of messages that the client is about to send
		memcpy(temp,buffer+4,strlen(buffer+4));
		int num=atoi(temp);
	    receive_messages(num,newsockfd);

	}

	//Case that the client asks for messages
	if (strncmp(temp,"read",4)==0)
	{
		//get the name of the client that asked for messages
		memcpy(name,buffer+4,2);
	    deliver_messages(name,newsockfd);
	}

	pthread_mutex_lock(&lock);
	thread_count-=1;
	pthread_mutex_unlock(&lock);
    close(newsockfd);

    return 0;
}



int main(int argc, char *argv[])
{
  int sockfd, newsockfd, portno, clilen;
  struct sockaddr_in serv_addr, cli_addr;
  int n,pid,status;
  pthread_t thread_id;

  if (argc < 2) {
    fprintf(stderr,"ERROR, no port provided\n");
    exit(1);
  }
  if (pthread_mutex_init(&lock, NULL) != 0)
  {
      error("Mutex init has failed\n");
  }
  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if (sockfd < 0)
    error("ERROR opening socket");
  bzero((char *) &serv_addr, sizeof(serv_addr));
  portno = atoi(argv[1]);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portno);
  if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
    error("ERROR on binding");
  listen(sockfd,5);
  clilen = sizeof(cli_addr);
  q = queueCreate();

  //Server listens for clients infinitely
  for(;;)
  {
		  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen); //Wait for a client to connect to server
		  if (newsockfd < 0)
			  error("ERROR on accept");

		  if(pthread_create(&thread_id,NULL,client_handler,(void*)&newsockfd)<0) //Create a thread to serve the client
		  {
			  error("Could not create thread");

		  }

  }
  return 0; //This line is never reached
}
