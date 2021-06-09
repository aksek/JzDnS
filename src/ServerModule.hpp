//
// Created by aniela on 6/6/21.
//

#ifndef JZDNS_SERVERMODULE_HPP
#define JZDNS_SERVERMODULE_HPP

#include "User.h"
#include "UserBase.h"
#include "message.h"
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <mutex>
#include <netinet/in.h>
#include "BlockingQueue.hpp"
#include "Logger.h"
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <thread>
#include "QueueMap.hpp"
#include "Looper.hpp"
#include <errno.h>

#define PORT 5011
#define BUFFER_SIZE 1024
#define ADDRESS "0.0.0.1"
#define MAXLINE     1024

class QueueMap;

class Looper;

class ServerModule {
private:
    enum class IP_Version {IPv4, IPv6};
    class ConnectionHandler
    {
    private:

        std::thread threadToSend;
        std::thread threadToReceive;
        ServerModule& sM;
        IP_Version version;
        struct sockaddr_in servaddr;
        int sockfd;

    public:
        ConnectionHandler(ServerModule& s, IP_Version v);
        void handle_connection_receive();
        void handle_connection_send();
        void joinThreadToSend();
        void joinThreadToReceive();
        void run();
        void stop();
//        ConnectionHandler(const ConnectionHandler &obj);
    };

//        std::vector<std::thread> mChildThreadsToSend;
//        std::vector<std::thread> mChildThreadsToReceive;
    ConnectionHandler handlerForIPv4;
    ConnectionHandler handlerForIPv6;
//    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool itsTimeToSayGoodbye{};
//    std::vector<std::reference_wrapper<ConnectionHandler>> connectionHandlers;
    QueueMap* userQueues;
    BlockingQueue<Message> mMessagesIPv4;
    BlockingQueue<Message> mMessagesIPv6;
    Looper* looper;
    Logger logger;

    std::map<std::string, IP_Version> user_version;
    std::map<std::string, struct sockaddr_in> user_address_IPv4;
    std::map<std::string, struct sockaddr_in> user_address_IPv6;


//    void runFunc();
    bool post(Message &&aMessage);


public:
    class Dispatcher {
        friend class ServerModule;
    private:
        ServerModule &mAssignedServerModule;
        explicit Dispatcher(ServerModule &aServer);
    public:
        bool post(Message &&aMessage);
        bool post_to_all(Message &&aMessage);
    };

    ServerModule()
    : mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
    , mRunning(false)
    , itsTimeToSayGoodbye(false)
    , mMessagesIPv4()
    , mMessagesIPv6()
//    , userQueues(queues)
    , looper(nullptr)
//    , connectionHandlers()
    , logger("ServerModule" + to_string(std::time(0)))
    , handlerForIPv4(*this, IP_Version::IPv4)
    , handlerForIPv6(*this, IP_Version::IPv6)
    {}
    ~ServerModule();

    void setLooper(Looper* aLooper);
    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();
    std::shared_ptr<Dispatcher> getDispatcher();

private:
    std::shared_ptr<Dispatcher> mDispatcher;
};



#endif //JZDNS_SERVERMODULE_HPP
