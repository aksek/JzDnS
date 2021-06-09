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


class QueueMap;

class Looper;

class ServerModule {
private:
    class ConnectionHandler
    {
    private:
        std::atomic_bool itsTimeToSayGoodbye;
        std::string userName;
        std::thread threadToSend;
        std::thread threadToReceive;
        ServerModule& sM;

    public:
        ConnectionHandler(ServerModule& s, int socket, sockaddr_in address);
        void handle_connection_receive(int socket, sockaddr_in address);
        void handle_connection_send(int socket, sockaddr_in address);
        void joinThreadToSend();
        void joinThreadToReceive();
        void run();
        void stop();
//        ConnectionHandler(const ConnectionHandler &obj);
    };

//        std::vector<std::thread> mChildThreadsToSend;
//        std::vector<std::thread> mChildThreadsToReceive;
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};
    std::vector<std::reference_wrapper<ConnectionHandler>> connectionHandlers;
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
    , mAbortRequested(false)
    , mMessages()
    , userQueues(queues)
    , looper(nullptr)
//    , connectionHandlers()
    , logger("ServerModule" + to_string(std::time(0)))
    {
    };
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
