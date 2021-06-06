//
//  author: Artur Mazur
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "src/message.h"
#include "src/Riddle.h"
#include "src/cryptography.hpp"
#include "src/UserBase.h"
#include "src/authorization.hpp"
#include "src/RiddleBase.h"
#include "src/RiddleModule.h"
#include "src/AdminModule.h"
#include "src/QueueMap.hpp"
#include "src/Looper.hpp"

#define PORT 4444
#define BUFFER_SIZE 1024
#define ADDRESS "127.0.0.1"
#define MAX_USERS 10

struct Info
{
	int socket;
	sockaddr_in address;
};

void * handle_connection(void * arguments)
{
	int socket = ((struct Info *)arguments)->socket;
	sockaddr_in adres = ((struct Info *)arguments)->address;

	char buffer[BUFFER_SIZE];
	while(1)
	{
		recv(socket, buffer, BUFFER_SIZE, 0);
		if(strcmp(buffer, ":exit") == 0)
		{
			printf("Disconnected from %s:%d\n", inet_ntoa(adres.sin_addr), ntohs(adres.sin_port));
			break;
		}
		else
		{
			printf("Client: %s\n", buffer);
			send(socket, buffer, strlen(buffer), 0);
			bzero(buffer, sizeof(buffer));
		}
	}

	return NULL;
}

int main()
{

	int sockfd, ret;
	 struct sockaddr_in serverAddr;

	int newSocket;
	struct sockaddr_in newAddr;

	socklen_t addr_size;

	char buffer[BUFFER_SIZE];
	pid_t childpid;

	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		buffer[i] = '\0';
	}

	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Server Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(PORT);
	serverAddr.sin_addr.s_addr = inet_addr(ADDRESS);

	ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in binding.\n");
		exit(1);
	}
	printf("[+]Bind to port %d\n", PORT);

	if(listen(sockfd, MAX_USERS) == 0){
		printf("[+]Listening....\n");
	}else{
		printf("[-]Error in binding.\n");
	}


	while(1)
    {
		newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
		if(newSocket < 0){
			exit(1);
		}
		printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

		pthread_t t;
		struct Info i =  {newSocket, newAddr};
		pthread_create(&t, NULL, handle_connection, &i);
	}

	close(newSocket);


	return 0;
}
