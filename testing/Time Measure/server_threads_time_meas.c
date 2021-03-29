/* Real-Time Embedded Systems - Project3: Client-Server Message Exchange Applications
 *
 * server_threads.c
 *
 *
 * This server code is enhanced so it can handle simultaneous connections,
 * that is multiple clients at a time. After a connection with a client is established,
 * a new thread is created to handle the communication. The server can either receive and save
 * messages from clients or deliver messages that are intended for them. A special structure is
 * used to handle message flow. A hash table that keeps the names of the clients and a linked list
 * associated to each client, representing client's mailbox. When the communication is over,
 * the thread returns. The server is running infinitely and awaits client requests.
 *
 * Author: Vasileios Matsoukas, Aristotle University of Thessaloniki
 * 		   Dept. of Electrical and Computer Engineering
 * Created on: Apr 19, 2018
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

#define MAX_CLIENTS 1000 //set the number of desired clients
#define MAX_UNQUEUED_CLIENTS 600

#include <stdbool.h>
#include "hash_list.h"
#include "queue.h"
struct queue *q;

pthread_mutex_t lock;

int thread_count=0;

void error(char *msg)
{
  perror(msg);
  exit(1);
}


void receive_messages(int num_msg,int sockfd) //save the messages that client sends to server
{
//	printf("---------Receive messages---------\n");
	char buffer[256];
	char tmp;
	int buflen,n,i;

//	printf("Num msg is: %d \n",num_msg);

	char **client_name=(char **)malloc(num_msg * sizeof(char*));
	char **message=(char **)malloc(num_msg * sizeof(char*));

	//struct Client** client=(struct Client**)malloc(num_msg*sizeof(struct Client *));
	//struct Client* client=(struct Client*)malloc(sizeof(struct Client));

	for(i=0; i<num_msg;i++)
	{
		client_name[i]=(char *)malloc(3*sizeof(char));
		message[i]=(char *)malloc(252* sizeof(char));
	}
	bzero(buffer,256);
	for(i=0; i<num_msg; i++)
	{
		//display_clients();
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
			client=search_client(client_name[i]); //----- APO edw
			if(client!=NULL)
			{
				//printf("Yparxei!\n");
			}
			else
			{
				//printf("Not found.\n");
				register_client(client_name[i]);
			}
			client=search_client(client_name[i]);// ews edw to thelei sigoura.
			if(client!=NULL)
			{
				//printf("Yparxei!\n");
			}
			else
			{
				//printf("Not found.\n");
				register_client(client_name[i]);
			}

//			display_clients();
//			printf("This message: %s is added to client %s\n", message[i], client_name[i]);
			client->message=add_message(client->message, message[i]);
//			printf("Client message -> %s \n", client->message->data);
			pthread_mutex_unlock(&lock);

			bzero(buffer,256);
			//sleep(1);
			break;
		}
	}

	//Send back an acknowledgement
	n = write(sockfd,"Messages succesfully delivered to server!\n",256);
	if (n < 0)
	   error("ERROR writing to socket");
}


void deliver_messages(char *client_name, int sockfd) //send messages saved in server to the client
{
//	printf("---------Deliver messages---------\n");
	char buffer[256];
	int n, num_msg;
	struct Client* client_temp=(struct Client *)malloc(sizeof(struct Client));

//	client_temp=search_client(client_name);
//	if(!is_client(client_name)) printf("Den einai client\n");

	pthread_mutex_lock(&lock); //
	client_temp=search_client(client_name);
	if(client_temp!=NULL)
	{
		printf("Client found. \n");
	}
	else
	{
		printf("Client %s does not exist.\n",client_name);
		return;
	}
//	display_clients();
//	print_message_list(client_temp->message);

	num_msg=list_length(client_temp->message);
	sprintf(buffer,"%d",num_msg);
	n=write(sockfd,buffer,256);
	fflush(stdout);
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
        //printf("This message will be sent back: %s*\n", buffer);
        n = write(sockfd,buffer,strlen(buffer));
        if (n < 0) error("ERROR writing to socket");
        bzero(buffer,256);

    }
    printf("\n");

    client_temp->message=free_message_list(client_temp->message);
    pthread_mutex_unlock(&lock);


	//Take acknowledgement that they were sent to client
	bzero(buffer,256);
	n = read(sockfd,buffer,255);
  //  printf("%s",buffer);
}


//Thread for client handling
void *client_handler(void *sock)
{
    pthread_mutex_lock(&lock);
	thread_count+=1;
	pthread_mutex_unlock(&lock);
	int tid=thread_count;
//    printf("Number of clients in system %d\n",thread_count );
    char buffer[256];
    char *temp=(char *)malloc(10*sizeof(char));
    char *name=(char *)malloc(3*sizeof(char));
    int n;
    int newsockfd=*(int *)sock;

	bzero(buffer,256);
	n = read(newsockfd,buffer,5);
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
			//queuePrint(q);
			//printf("Peek(q) is %d \n", peek(q));
			if(thread_count>MAX_UNQUEUED_CLIENTS)
			{
				//printf("eimai edw. %d \n",tid);
				usleep(50000); //ta sleeps prepei na mikrunoun
			}
			else
			{
				if(peek(q)==tid)
				{
					//printf("Eimai o %d kai vgainw apo thn oura\n",tid);

					pthread_mutex_lock(&lock);
					int c=deq(q);
					pthread_mutex_unlock(&lock);
					break;
				}
				else
				{
					usleep(50000); //ta sleeps prepei na mikrunoun
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
		printf("num is %d\n",num);
	    receive_messages(num,newsockfd);

	}

	//Case that the client asks for messages
	if (strncmp(temp,"read",4)==0)
	{
		n = read(newsockfd,buffer+5,1);
		memcpy(temp,buffer,strlen(buffer));
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
