#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>


#include"ServerModule.hpp"
#include "QueueMap.hpp"
#include "Looper.hpp"

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
    logger.write("Start");

    int sockfd, ret;
    struct sockaddr_in serverAddr;

    int newSocket;
    struct sockaddr_in newAddr;

    socklen_t addr_size;

    char buffer[BUFFER_SIZE];


    for(int i = 0; i < BUFFER_SIZE; ++i)
    {
        buffer[i] = '\0';
    }

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd < 0){
        logger.write("[-]Error in connection.");
        exit(1);
    }
    logger.write("[+]Server Socket is created.");

    memset(&serverAddr, '\0', sizeof(serverAddr));
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(PORT);
    serverAddr.sin_addr.s_addr = inet_addr(ADDRESS);


    ret = bind(sockfd, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
    if(ret < 0){
        logger.write("[-]Error in binding.");
        exit(1);
    }
    logger.write("[+]Bind to port " + to_string(PORT));

    if(listen(sockfd, MAX_USERS) == 0){
        logger.write("[+]Listening....");
    }else{
        logger.write("[-]Error in binding.");
    }

    while(!mAbortRequested.load()) {
        struct timeval waitThreshold;
        waitThreshold.tv_sec = 2;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);
        ret = select(sockfd + 1, &readfds, NULL, NULL, &waitThreshold);
        if(ret > 0)
        {
            newSocket = accept(sockfd, (struct sockaddr*)&newAddr, &addr_size);

            logger.write(std::string("Connection accepted from ") + std::string(inet_ntoa(newAddr.sin_addr)) + " : " + to_string(ntohs(newAddr.sin_port)));

            if(newSocket < 0)
                continue;

            ConnectionHandler cosik(*this, newSocket, newAddr);
            connectionHandlers.push_back(cosik);
        }
        else if(ret == 0)
            continue;
   }
    logger.write("Finish");
    mRunning.store(false);
}

bool ServerModule::post(Message &&aMessage) {
    if (not running()) {
        return false;
    }

    try {
        if (user_version[aMessage.getUserID()] == IP_Version::IPv4) {
            mMessagesIPv4.push(aMessage);
        } else if (user_version[aMessage.getUserID()] == IP_Version::IPv6) {
            mMessagesIPv6.push(aMessage);
        }
    } catch (...) {
        return false;
    }
    return true;
}

void ServerModule::ConnectionHandler::handle_connection_receive(int socket, sockaddr_in address)
{
    char buffer[BUFFER_SIZE];
    while(!sM.mAbortRequested.load()) {
        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(socket, &readfds);
        int ret = select(socket + 1, &readfds, NULL, NULL, &tv);

        if(ret > 0) {
            recv(socket, buffer, BUFFER_SIZE, 0);
            if (strcmp(buffer, ":exit") == 0) {
                sM.logger.write(std::string("Disconnected from ") + std::string(inet_ntoa(address.sin_addr)) + " : " + to_string(ntohs(address.sin_port)));
                itsTimeToSayGoodbye.store(true);
                break;
            }
            else
            {
                sM.logger.write("Received message");
//            send(socket, buffer, strlen(buffer), 0);
                Message message((std::string(buffer)));
                std::string username = message.getUserID();
                if (!sM.userQueues->isInMap(username)) sM.userQueues->add_user(username, new BlockingQueue<Message>());

                sM.looper->getDispatcher()->post(Message((std::string(buffer))));

                bzero(buffer, sizeof(buffer));
            }
        }
        if(ret == 0)
            continue;

    }
}

void ServerModule::ConnectionHandler::handle_connection_send(int socket, sockaddr_in address) {
    char buffer[BUFFER_SIZE];
    while(!sM.mAbortRequested.load() || !itsTimeToSayGoodbye) {
        Message next = sM.userQueues->pop(userName);
        if (next.getUserID().empty()) {
            continue;
        }
        sM.logger.write("Sending message: " + next.getMessageTypeString());

        std::string serialized_message = next.serialize();

        sockaddr_in cliaddr;
        if (version == IP_Version::IPv4) {
            cliaddr = sM.user_address_IPv4[next.getUserID()];

        } else if (version == IP_Version::IPv6) {
            cliaddr = sM.user_address_IPv6[next.getUserID()];

        } else continue;

        send(socket, serialized_message.c_str(), strlen(serialized_message.c_str()), 0);
    }
}

ServerModule::ConnectionHandler::ConnectionHandler(ServerModule &s, int socket, sockaddr_in address)
: sM(s)
, version(v) {

}
/*
ServerModule::ConnectionHandler::~ConnectionHandler()
{
    threadToReceive.join();
    threadToSend.join();
}
*/

void ServerModule::ConnectionHandler::joinThreadToSend()
{
    threadToSend.join();
}

void ServerModule::ConnectionHandler::joinThreadToReceive()
{
    threadToReceive.join();
}
/*
ServerModule::ConnectionHandler::ConnectionHandler(const ServerModule::ConnectionHandler &obj) : sM(obj.sM){
    this->itsTimeToSayGoodbye.store(obj.itsTimeToSayGoodbye.load());
    this->userName = obj.userName;
    this->threadToSend = std::move(obj.threadToSend);// .store(obj.threadToSend.load());
    this->threadToReceive = std::move(obj.threadToReceive);
}
*/
ServerModule::~ServerModule() {
    for(auto &handler : connectionHandlers)
    {
        handler.get().joinThreadToSend();
        handler.get().joinThreadToReceive();
    }
*/
//    connectionHandlers.clear();
    abortAndJoin();
}

void ServerModule::setLooper(Looper *aLooper) {
    looper = aLooper;
}

bool ServerModule::run() {
    try
    {
//        mThread = std::thread(&ServerModule::runFunc, this);

        handlerForIPv4.run();
        handlerForIPv6.run();

    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool ServerModule::running() const {
    return mRunning.load();
}

void ServerModule::stop() {
    abortAndJoin();
}

void ServerModule::abortAndJoin() {
    mAbortRequested.store(true);
    if (mThread.joinable()) {
        mThread.join();
    }
}

void ServerModule::ConnectionHandler::stop() {
    if (threadToReceive.joinable()) {
        threadToReceive.join();
    }
    if (threadToSend.joinable()) {
        threadToSend.join();
    }
}


std::shared_ptr<ServerModule::Dispatcher> ServerModule::getDispatcher() {
    return mDispatcher;
}

ServerModule::Dispatcher::Dispatcher(ServerModule &aServer)
: mAssignedServerModule(aServer){

}

bool ServerModule::Dispatcher::post(Message &&aMessage) {
    return mAssignedServerModule.post(std::move(aMessage));
}
bool ServerModule::Dispatcher::post_to_all(Message &&aMessage) {
    for (auto user_v : mAssignedServerModule.user_version) {
        Message message(aMessage.getMessageType(), user_v.first, aMessage.getContent());
        if (!mAssignedServerModule.post(std::move(message))) return false;
    }
    return true;
}

void ServerModule::ConnectionHandler::run() {

    if( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0)
    {
        sM.logger.write("socket creation failed");
        sM.itsTimeToSayGoodbye.store(true);
    }

    //tworzenie adresu
    // servaddr.sin_family = (version == IP_Version::IPv4) ? AF_INET : AF_INET6; // IPv4
    servaddr.sin_family = AF_INET; // IPv4
    servaddr.sin_addr.s_addr = inet_addr(ADDRESS);
    servaddr.sin_port = htons(PORT);

    if ( bind(sockfd, (const struct sockaddr *)&servaddr,
              sizeof(servaddr)) < 0 )
    {
        sM.logger.write("bind failed");
        sM.logger.write("Oh dear, something went wrong with read()! %s\n", strerror(errno));
        sM.itsTimeToSayGoodbye.store(true);
    }

    threadToSend = std::thread(&ServerModule::ConnectionHandler::handle_connection_send, this);
    threadToReceive = std::thread(&ServerModule::ConnectionHandler::handle_connection_receive, this);
}

