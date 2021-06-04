//
// Created by aniela on 6/3/21.
//

#include "Looper.hpp"

void Looper::runFunc() {
    mRunning.store(true);

    while(false == mAbortRequested.load()) {
        Message next(MessageType::OK, "");
        mMessages.waitAndPop(next);

        switch(next.getMessageType()) {
            case MessageType::Retransmit:
            case MessageType::Problem:
            case MessageType::Correct:
            case MessageType::Problems:
            case MessageType::OK:
            case MessageType::Login_OK:
            case MessageType::Login_error:
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
                riddleService->getDispatcher()->post(std::move(next));
                break;
            case MessageType::New_problem:
            case MessageType::Delete_problem:
            case MessageType::Edit_problem:
            case MessageType::Edit_solution:
            case MessageType::Get_all_problems:
                adminService->getDispatcher()->post(std::move(next));
                break;
            default:
                break;
        }
    }

    mRunning.store(false);
}

Looper::Looper(QueueMap* userQueues, Authorization* authorization, RiddleService* riddleService, AdminService* adminService)
: mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this)))
, mRunning(false)
, mAbortRequested(false)
, mMessages()
, authorization(authorization)
, riddleService(riddleService)
, adminService(adminService)
, userQueues(userQueues)
{
    authorization->run();
    // TODO run the rest
}

Looper::~Looper() {
    authorization->stop();
    // TODO stop the rest
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
