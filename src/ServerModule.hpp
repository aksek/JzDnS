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
#include "BlockingQueue.hpp"

class Looper;

class ServerModule {
private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

    BlockingQueue<Message> mMessages;
    Looper* looper;

    UserBase* base;
    SerializeContent* serializer;

    void runFunc();
    bool post(Message &&aMessage);

    void * handle_connection(void * arguments);
public:
    class Dispatcher {
        friend class Server;
    private:
        ServerModule &mAssignedServerModule;
        explicit Dispatcher(ServerModule &aServer);
    public:
        bool post(Message &&aMessage);
    };

    ServerModule();
    ~ServerModule();

    void setLooper(Looper* looper);
    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();
    std::shared_ptr<Dispatcher> getDispatcher();

private:
    std::shared_ptr<Dispatcher> mDispatcher;
};


#endif //JZDNS_SERVERMODULE_HPP
