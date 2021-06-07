#include "listeningClient.h"

listeningClient::listeningClient(int p, std::string a, int bs)
{
    port = p;
    address = a;
    buffersize = bs;
}

void listeningClient::run()
{
    int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[buffersize];

    int n = address.length();
    char char_addr[n+1];
    strcpy(char_addr, address.c_str());

	for(int i = 0; i < buffersize; ++i)
	{
		buffer[i] = '\0';
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(char_addr);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");


	while(1)
    {
		printf("Client: \t");
		scanf("%s", &buffer[0]);
		send(clientSocket, buffer, strlen(buffer), 0);

		if(strcmp(buffer, ":exit") == 0){
			close(clientSocket);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}

		if(recv(clientSocket, buffer, buffersize, 0) < 0){
			printf("[-]Error in receiving data.\n");
		}else{
			printf("Server: \t%s\n", buffer);
		}
	}
}