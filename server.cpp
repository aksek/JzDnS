//
//  author: Artur Mazur
//

#include <stdio.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>

#define SERVER_PORT 3006
#define BUFFER_LENGTH 250


int main()
{
    int sd=-1, sdconn=-1;
    int on=1;
    int rc;
    struct sockaddr_in6 serveraddr, clientaddr;
    int addrlen=sizeof(clientaddr);
    char str[INET6_ADDRSTRLEN];
    char buffer[BUFFER_LENGTH];
    int rcdsize = BUFFER_LENGTH;

    do
    {
        if((sd = socket(AF_INET6, SOCK_STREAM, 0) )< 0)
        {
            perror("socket() failed");
            break;
        }

        if((setsockopt(sd, SOL_SOCKET, SO_REUSEADDR, (char *)&on, sizeof(on))) < 0)
        {
            perror("setsockopt(SO_REUSEADDR) faile");
            break;
        }

        memset(&serveraddr, 0, sizeof(serveraddr));
        serveraddr.sin6_family = AF_INET6;
        serveraddr.sin6_port = htons(SERVER_PORT);
        serveraddr.sin6_addr = in6addr_any;

        if( bind(sd, (struct sockaddr *)&serveraddr, sizeof(serveraddr)) < 0)
        {
            perror("bind() failed)");
            break;
        }

        if(listen(sd, 10) < 0)
        {
            perror("listen() failed");
            break;
        }

        printf("Ready for client connect()\n");

        if((sdconn = accept(sd, NULL, NULL)) < 0)
        {
            perror("accept() failed");
            break;
        }
        else
        {
            getpeername(sdconn, (struct sockaddr *)&clientaddr, (socklen_t*)&addrlen);
            if(inet_ntop(AF_INET6, &clientaddr.sin6_addr, str, sizeof(str)))
            {
                printf("Client address is %s\n", str);
                printf("Client port is %d\n", ntohs(clientaddr.sin6_port));
            }
        }

        if(setsockopt(sdconn, SOL_SOCKET, SO_RCVLOWAT, (char *)&rcdsize, sizeof(rcdsize)) < 0)
        {
            perror("setsockpot(SO_RCVLOWAT) failed");
            break;
        }

        rc = recv(sdconn, buffer, sizeof(buffer), 0);
        if(rc < 0 || rc < sizeof(buffer))
        {
            printf("The client closed the connection before  all of the data was sent\n");
            break;
        }

        rc = send(sdconn, buffer, sizeof(buffer), 0);
        if(rc < 0)
        {
            perror("send() failed");
            break;
        }

        write(1, buffer, rc);

    } while (0);

    if(sd != -1)
        close(sd);
    if(sdconn != -1)
        close(sdconn);
    
}