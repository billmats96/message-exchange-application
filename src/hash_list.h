/*
 * hash_list.h
 *
 *  Created on: Sep 22, 2018
 *      Author: ubuntu
 */

#ifndef HASH_LIST_H_
#define HASH_LIST_H_



/*********** Linked list features ***********/

typedef struct node
{
    char* data;
    //int key;
    struct node* next;
} NODE;

void init_message_list(NODE** head)
{
    *head = NULL;
}

NODE* register_list()
{
	NODE *new_list=(NODE *)malloc(sizeof(NODE));
	init_message_list(&new_list);
	return new_list;
}

int list_length(NODE* head)
{

    NODE * temp;
    int length=0;
    for (temp = head; temp; temp = temp->next)
    {
       length++;
    }
    return length;
}

void print_message_list(NODE* head)
{

    NODE * temp;
    for (temp = head; temp; temp = temp->next)
    {
        printf("[ %s ]", temp->data);

    }
    printf("\n");
}

NODE* add_message(NODE* node, char *data)
{
    NODE* temp = (NODE*) malloc(sizeof (NODE));
    if (temp == NULL)
    {
        exit(0); // no memory available
    }
    temp->data = data;
    temp->next = node;
    node = temp;
    return node;
}

void add_message_at(NODE* node, char* data)
{
    NODE* temp = (NODE*) malloc(sizeof (NODE));
    if (temp == NULL)
    {
        exit(EXIT_FAILURE); // no memory available
    }
    temp->data = data;
    temp->next = node->next;
    node->next = temp;
}

NODE* remove_message(NODE* head)
{
    NODE* temp = (NODE*) malloc(sizeof (NODE));
    if (temp == NULL)
    {
        exit(EXIT_FAILURE); // no memory available
    }
    //temp = head->next;
    //head->next = head->next->next;
    temp = head;
    if(head->next!=NULL)
    	head = head->next;

    free(temp);
    return head;
}

NODE * reverse_rec(NODE * ptr, NODE * previous)
{
    NODE * temp;
    if (ptr->next == NULL)
    {
        ptr->next = previous;
        return ptr;
    }
    else
    {
        temp = reverse_rec(ptr->next, ptr);
        ptr->next = previous;
        return temp;
    }
}

NODE * reverse(NODE * node)
{
    NODE * temp;
    NODE * previous = NULL;
    while (node != NULL)
    {
        temp = node->next;
        node->next = previous;
        previous = node;
        node = temp;
    }
    return previous;
}

NODE *free_message_list(NODE *head)
{
    NODE *tmpPtr = head;
    NODE *followPtr;
    while (tmpPtr != NULL)
    {
        followPtr = tmpPtr;
        tmpPtr = tmpPtr->next;
        free(followPtr);
    }
    return NULL;
}


/*********** Hash table features ***********/

struct Client
{
   NODE* message;
   char* key;
};


struct Client* hashArray[MAX_CLIENTS];
struct Client* dummyClient = NULL;
struct Client* client;


struct Client *search_client(char* key)
{
	int hashIndex=0;

	//move in array until an empty
   while(hashArray[hashIndex] != NULL)
   {

	   if(hashArray[hashIndex]->key[0]==key[0] && hashArray[hashIndex]->key[1]==key[1]){

		   return hashArray[hashIndex];
	   }
      if(hashArray[hashIndex]->key == key)
      {
    	  return hashArray[hashIndex];
      }
      //go to next cell
      ++hashIndex;
      //wrap around the table
      hashIndex %= MAX_CLIENTS;
   }
   return NULL;
}

void insert_client(char* key,NODE* message) {

   struct Client *client = (struct Client*) malloc(sizeof(struct Client));
   client->message = message;
   client->key = key;

   int hashIndex=0;
   //move in array until an empty or deleted cell
   while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key !=NULL /*&& hashArray[hashIndex]->key != -1*/)
   {
      //go to next cell
      ++hashIndex;

      //wrap around the table
      hashIndex %= MAX_CLIENTS;
   }

   hashArray[hashIndex] = client;
}

struct Client* delete_client(struct Client* client) {
   char* key = client->key;

   int hashIndex=0;
   //move in array until an empty
   while(hashArray[hashIndex]!=NULL)
   {
      if(strcmp(hashArray[hashIndex]->key , key))
      {
         struct Client* temp = hashArray[hashIndex];

         //assign a dummy CLIENT at deleted position
         hashArray[hashIndex] = dummyClient;
         return temp;
      }
      //go to next cell
      ++hashIndex;
      //wrap around the table
      hashIndex %= MAX_CLIENTS;
   }
   return NULL;
}

//Display hash table
void display_clients()
{
   int i=0;
   for(i=0; i<MAX_CLIENTS; i++)
   {
      if(hashArray[i] != NULL)
      {
    	 if(hashArray[i]->message!=NULL)
    	 {
    		 printf(" (%s,%s)",hashArray[i]->key,hashArray[i]->message->data);
    	 }
    	 else
    	 {
    		 printf(" (%s,~~)",hashArray[i]->key);
    	 }
      }
      else
      {
         printf(" ~~ ");
      }
   }
   printf("\n");
}


int is_client(char *client_name)
{
	struct Client *client=search_client(client_name);

	if(client!=NULL)
	{
		return strcmp(client->key,client_name);
	}
	else
	{
		return 0;
	}

}
void register_client(char *client_name)
{
	insert_client(client_name,register_list());
}


#endif /* HASH_LIST_H_ */
