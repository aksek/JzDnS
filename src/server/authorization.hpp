//
// Created by aniela on 5/22/21.
//
#pragma once

#include "User.h"
#include "UserBase.h"
#include "../message.h"
#include <thread>
#include <atomic>
#include <memory>
#include <functional>
#include <stdexcept>
#include <mutex>
#include "../BlockingQueue.hpp"

class QueueMap;
class Looper;

class Authorization {
private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

//    QueueMap* userQueues;
    BlockingQueue<Message> mMessages;
    Looper* looper;

    UserBase* base;
    SerializeContent* serializer;

    void runFunc();
    bool post(Message &&aMessage);

    bool authorize(std::string username);
    bool authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key);

    void handleLogin(ValueContent content, std::string user);
    void handleRegister(ValueContent content, std::string user);

public:
    class Dispatcher {
        friend class Authorization;
    private:
        Authorization &mAssignedAuthorization;
        explicit Dispatcher(Authorization &aAuthorization);
    public:
        bool post(Message &&aMessage);
    };

    Authorization(UserBase *user_base);
    ~Authorization();

    void setLooper(Looper* looper);
    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();
    std::shared_ptr<Dispatcher> getDispatcher();

    int determineUserType(std::string username);
    CryptoPP::RSA::PublicKey getKey(std::string username);

private:
    std::shared_ptr<Dispatcher> mDispatcher;

    void sendResponse(std::string &user);
};

