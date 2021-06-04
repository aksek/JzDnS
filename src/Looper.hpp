//
// Created by aniela on 6/3/21.
//
#pragma once

#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <mutex>
#include <map>
//#include "BlockingQueue.hpp"
#include "message.h"
#include "authorization.hpp"
#include "RiddleService.h"
#include "AdminService.h"
#include "QueueMap.hpp"


class Authorization;

class Looper {

private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

    QueueMap* userQueues;
    BlockingQueue<Message> mMessages;

    Authorization* authorization;
    RiddleService* riddleService;
    AdminService* adminService;

    void runFunc();
    bool post(Message &&aMessage);

public:
    class Dispatcher {
        friend class Looper;
    private:
        Looper &mAssignedLooper;
        explicit Dispatcher(Looper &aLooper);
    public:
        bool post(Message &&aMessage);
    };

    Looper(QueueMap* userQueues, Authorization* authorization, RiddleService* riddleService, AdminService* adminService);
    ~Looper();

    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();

    std::shared_ptr<Dispatcher> getDispatcher();
private:
    std::shared_ptr<Dispatcher> mDispatcher;
};



