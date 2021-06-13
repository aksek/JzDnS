//
// Created by aniela on 6/3/21.
//
#pragma once

#include <cryptopp/osrng.h>
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <mutex>
#include <map>
#include "../message.h"
#include "authorization.hpp"
#include "RiddleService.h"
#include "AdminService.h"
#include "../QueueMap.hpp"
#include "RiddleModule.h"
#include "AdminModule.h"

class ServerModule;
class Authorization;

class Looper {

private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

    QueueMap* userQueues;
    BlockingQueue<Message> mMessages;

    Authorization* authorization;
    AdminModule* adminModule;
    RiddleModule* riddleModule;
    ServerModule* serverModule;

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey public_key;
    CryptoPP::RSA::PrivateKey private_key;

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

    Looper(Authorization* authorization, RiddleModule* riddleModule, AdminModule* adminModule, ServerModule* serverModule);
    ~Looper();

    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();

    std::shared_ptr<Dispatcher> getDispatcher();
private:
    std::shared_ptr<Dispatcher> mDispatcher;
};



