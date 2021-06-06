#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

#include"ServerModule.hpp"

#define PORT 4444
#define BUFFER_SIZE 1024
#define ADDRESS "127.0.0.1"
#define MAX_USERS 10

struct Info
{
    int socket;
    sockaddr_in address;
};

void ServerModule::runFunc() {
    mRunning.store(true);

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

    while(!mAbortRequested.load()) {
        newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);
        if(newSocket < 0){
            exit(1);
        }
        printf("Connection accepted from %s:%d\n", inet_ntoa(newAddr.sin_addr), ntohs(newAddr.sin_port));

        pthread_t t;
        struct Info i =  {newSocket, newAddr};
        pthread_create(&t, NULL, handle_connection, &i);
    }
}

bool ServerModule::post(Message &&aMessage) {
    return false;
}

void *ServerModule::handle_connection(void *arguments) {
    return nullptr;
}

ServerModule::ServerModule() {

}

ServerModule::~ServerModule() {

}

void ServerModule::setLooper(Looper *looper) {

}

bool ServerModule::run() {
    return false;
}

bool ServerModule::running() const {
    return false;
}

void ServerModule::stop() {

}

void ServerModule::abortAndJoin() {

}

std::shared_ptr<Dispatcher> ServerModule::getDispatcher() {
    return std::shared_ptr<Dispatcher>();
}
