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
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};
    std::vector<std::reference_wrapper<ConnectionHandler>> connectionHandlers;
    QueueMap* userQueues;
    BlockingQueue<Message> mMessages;
    Looper* looper;
    Logger logger;


    void runFunc();
    bool post(Message &&aMessage);

    void * handle_connection(void * arguments);
public:
    class Dispatcher {
        friend class ServerModule;
    private:
        ServerModule &mAssignedServerModule;
        explicit Dispatcher(ServerModule &aServer);
    public:
        bool post(Message &&aMessage);
    };

    ServerModule(QueueMap* queues)
    : mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
    , mRunning(false)
    , mAbortRequested(false)
    , mMessages()
    , userQueues(queues)
    , looper(nullptr)
    , connectionHandlers()
    , logger("ServerModule")
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
