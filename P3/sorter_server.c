#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <pthread.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>

/*
 * sort when asked to dumb
 */
int headerDigitCount(int c_s);
void getRecord(char* record, int c_s,int length);
int byteCount(int c_s,int digitCount);
//-----------------------------------------------------
int main() {
	int SERVER_PORT = 8876;
	int server_socket = socket(AF_INET,SOCK_STREAM,0);

	if (server_socket == 0)
	{
		printf("Error creating socket on server side faild\n");
		exit(EXIT_FAILURE);
	}

// socket address used for the server

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(SERVER_PORT);
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

// Binding
	if(bind(server_socket, (struct sockaddr*)server_address ,sizeof(server_address))<0)
	{
		printf("Failed to bind socket\n");
		exit(EXIT_FAILURE);
	}

// Listening waiting time 16
	if(listen(server_socket,16)<0)
	{
		printf("Listening to socket failed\n");
		exit(EXIT_FAILURE);
	}

int client_socket;
	client_socket = accept(server_socket,NULL,NULL);
	if(client_socket == -1)
	{
		printf("Problem creating connection to socket %s\n",strerror(errno));
		exit(EXIT_FAILURE);
	}
char *recieved = "Recieved record.";
char *ack = "Recording";
char *sorts = "Sorting";
char *ret = "Returning";



// record transfer protocol
int exit = 0;
char buffer[8];
while(strcmp(exit == 0))
{	
	readvalue = recv(client_socket,buffer,7)
	buffer[7] = '/0';
	if(strcmp(buffer,"record")==0)
		{
			while(condition == 0)
			{
			
				send(client_socket,ack,strlen(ack));
				int headerLength;
				int messageLength;
				char* tempRec;

				headerLength = headerDigitCount(client_socket);
					if(headerLength<=0)
						{
							condition = -1;
						}
					else
						{

							messageLength = byteCount(client_socket,headerLength);
							tempRec = malloc(sizeof(char)*messageLength);
							getRecord(client_socket,tempRec,messageLength);
							send(client_socket,recieved,strlen(recieved));
							//add parse function here
						}
			}
		}
	if(strcmp(buffer,"sort")==0)
	{		
		//
	//sort
		//
	}
	if(strcmp(buffer,"return")==0)
	{
		//
	//return
		//
		exit = 1;
	}


}
return 0;



}
//--------------------------------------------------
void getRecord(int c_s,char* record,int length)
{
	int counter = 0;
	int readvalue = 0;
	while(counter<length)
	{
		readvalue = recv(c_s,record+counter,length-counter);
		counter = readvalue;	
	}

return;
}

int byteCount(int c_s,int digitCount)
{	
char* buffer = (char*) malloc(sizeof(char)*digitCount);

int readvalue = 0;
while(readvalue==0)
	readvalue = recv(c_s,buffer,1);
readvalue = 0;
readvalue = recv(c_s,buffer,digitCount);
	if(readvalue == digitCount)
	{
		return atoi(buffer);
	}
	else
	{
		printf("%s\n","Error has occured when reading header" );
		exit(EXIT_FAILURE);
		return 0;
	}
}

int headerDigitCount(int c_s,)
{
	// read header digit count
char* buffer = (char*) malloc(sizeof(char)*9);
int exit_condition = 1;
int buffer_tracker = 0;
int readvalue;
while(exit_condition == 1)
{

readvalue = recv(c_s,buffer+buffer_tracker,1);
	if(readvalue == 1)
	{
		buffer_tracker++;
	}
	if(buffer[buffer_tracker] = '@')
	{
		exit_condition = 0;
		buffer[buffer_tracker] = '\0';
	}
}
return atoi(buffer);
}