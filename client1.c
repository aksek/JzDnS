#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
  
#define PORT        4470
#define MAXLINE     1024
#define ADDRESS     "127.0.0.1"
  
// Driver code
int main() {
    int sockfd;
    char buffer[MAXLINE];
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
  
    // Creating socket file descriptor
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));
      
    // Filling server information
    servaddr.sin_family = AF_INET;
    servaddr.sin_port = htons(PORT);
    servaddr.sin_addr.s_addr = inet_addr(ADDRESS);
      
    int n;
    socklen_t len = sizeof(servaddr); 

    while(1)
    {
        printf("Client: \t");
		scanf("%s", &buffer[0]);
        
        sendto(sockfd, (char *)buffer, strlen(hello),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));
        
        if(strcmp(buffer, ":exit") == 0){
			close(sockfd);
			printf("[-]Disconnected from server.\n");
			exit(1);
		}
            
        n = recvfrom(sockfd, (char *)buffer, MAXLINE, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);
        buffer[n] = '\0';
        printf("Server : %s\n", buffer);
    }

}