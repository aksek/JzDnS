//
// Created by patryk on 04.06.2021.
//

#include "Looper.hpp"
#include "RiddleModule.h"
#include "Logger.h"

bool RiddleModule::post(Message &&aMessage)
{
    if(not running())
    {
        return false;
    }

    try {
        messagesQueue.push(aMessage);
    }
    catch(...)
    {
        return false;
    }
    return true;
}

RiddleModule::RiddleModule(RiddleBase* riddleBase) :
        mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this))),
        mRunning(false),
        mAbortRequested(false),
        messagesQueue(),
        riddleService(riddleBase),
        serializer()
{}

void RiddleModule::setLooper(Looper* looper)
{
    this->looper = looper;
}

void RiddleModule::handleGetSolution(ValueContent content, std::string user)
{
    if(!std::holds_alternative<std::string>(content))
    {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }
    std::string answer = std::get<std::string>(content);

    Riddle riddle = riddleService.getLastRiddle();

    if(riddle.getAnswer() == answer)
    {
        looper->getDispatcher()->post(Message(MessageType::Correct, user, serializer->serializeBool(true)));
        looper->getDispatcher()->post(Message(MessageType::Round_over, user));
        riddleService.getNextRiddle();
    }
    else
    {
        looper->getDispatcher()->post(Message(MessageType::Correct, user, serializer->serializeBool(false)));
        return;
    }
}

void RiddleModule::handleGetProblem(std::string user)
{
    Riddle riddle = riddleService.getLastRiddle();

    looper->getDispatcher()->post(Message(MessageType::Problem, user, serializer->serializeString(riddle.getRiddleContent())));
}

void RiddleModule::runFunc()
{
    mRunning.store(true);
    Logger logger("RiddleModule" + to_string(std::time(0)));
    logger.write("Start");

    while (!mAbortRequested.load())
    {
        Message next(MessageType::OK, "");
//        messagesQueue.waitAndPop(next);
        if (!messagesQueue.tryWaitAndPop(next, 2000)) {
            logger.write("Timeout");
            continue;
        }
        logger.write("Received " + next.getMessageTypeString() + " : " + std::string(next.getUserID()));

        MessageType type = next.getMessageType();
        ValueContent content = serializer->deserialize(type, next.getContent());

        switch (type) {
            case MessageType::Solution:
                handleGetSolution(content, next.getUserID());
                break;
            case MessageType::Get_current_problem:
                handleGetProblem(next.getUserID());
                break;
            default:
                break;
        }
    }
    logger.write("Finish");
    mRunning.store(false);
}

RiddleModule::~RiddleModule()
{
    abortAndJoin();
}

bool RiddleModule::run()
{
    try {
        mThread = std::thread(&RiddleModule::runFunc, this);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool RiddleModule::running() const
{
    return mRunning.load();
}

void RiddleModule::stop()
{
    abortAndJoin();
}

void RiddleModule::abortAndJoin()
{
    mAbortRequested.store(true);
    if(mThread.joinable())
    {
        mThread.join();
    }
}

std::shared_ptr<RiddleModule::Dispatcher> RiddleModule::getDispatcher()
{
    return mDispatcher;
}

RiddleModule::Dispatcher::Dispatcher(RiddleModule &riddleModule) : riddleModule(riddleModule){}

bool RiddleModule::Dispatcher::post(Message &&message)
{
    return riddleModule.post(std::move(message));
}