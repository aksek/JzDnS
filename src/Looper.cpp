//
// Created by aniela on 6/3/21.
//

#include "Looper.hpp"

void Looper::runFunc() {
    mRunning.store(true);

    while(false == mAbortRequested.load()) {
        Message next(MessageType::OK, "");

        if (!mMessages.tryWaitAndPop(next, 2000)) {
//            mRunning.store(false);
            continue;
        }

        switch(next.getMessageType()) {
            case MessageType::Retransmit:
            case MessageType::Problem:
            case MessageType::Correct:
            case MessageType::Problems:
            case MessageType::OK:
                userQueues->post_to(next.getUserID(), next);
                break;
            case MessageType::Round_over:
                userQueues->post_except(next.getUserID(), next);
                break;
            case MessageType::Login:
            case MessageType::Register:
                authorization->getDispatcher()->post(std::move(next));
                break;
            case MessageType::Solution:
            case MessageType::Get_current_problem:
                riddleModule->getDispatcher()->post(std::move(next));
                break;
            case MessageType::New_problem:
            case MessageType::Delete_problem:
            case MessageType::Update:
            case MessageType::Get_all_problems:
                adminModule->getDispatcher()->post(std::move(next));
                break;
            default:
                break;
        }
    }

    mRunning.store(false);
}

Looper::Looper(QueueMap* userQueues, Authorization* authorization, RiddleModule* riddleModule, AdminModule* adminModule)
: mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
, mRunning(false)
, mAbortRequested(false)
, mMessages()
, authorization(authorization)
, riddleModule(riddleModule)
, adminModule(adminModule)
, userQueues(userQueues)
{
    authorization->setLooper(this);
    authorization->run();
    adminModule->setLooper(this);
    adminModule->run();
    riddleModule->setLooper(this);
    riddleModule->run();
}

Looper::~Looper() {
    authorization->stop();
    adminModule->stop();
    riddleModule->stop();
    abortAndJoin();
}

bool Looper::run() {
    try
    {
        mThread = std::thread(&Looper::runFunc, this);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool Looper::running() const {
    return mRunning.load();
}

void Looper::stop() {
    abortAndJoin();
}

void Looper::abortAndJoin() {
    mAbortRequested.store(true);
    if (mThread.joinable()) {
        mThread.join();
    }
}

std::shared_ptr<Looper::Dispatcher> Looper::getDispatcher() {
    return mDispatcher;
}

bool Looper::post(Message &&aMessage) {
    if (not running()) {
        return false;
    }

    try {
        mMessages.push(aMessage);
    } catch (...) {
        return false;
    }
    return true;
}

Looper::Dispatcher::Dispatcher(Looper &aLooper)  : mAssignedLooper(aLooper) {}

bool Looper::Dispatcher::post(Message &&aMessage) {
    return mAssignedLooper.post(std::move(aMessage));
}
