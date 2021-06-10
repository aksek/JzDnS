//
// Created by patryk on 03.06.2021.
//

#include "Looper.hpp"
#include "AdminModule.h"
#include "../Logger.h"

bool AdminModule::post(Message &&aMessage)
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

AdminModule::AdminModule(RiddleBase* riddleBase) :
mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this))),
mRunning(false),
mAbortRequested(false),
messagesQueue(),
adminService(riddleBase),
serializer()
{}

void AdminModule::runFunc()
{
    mRunning.store(true);
    Logger logger("AdminModule" + to_string(std::time(0)));
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
            case MessageType::Get_all_problems:
                handleGetAllRiddles(next.getUserID());
                break;
            case MessageType::New_problem:
                handleAddNewRiddle(content, next.getUserID());
                break;
            case MessageType::Delete_problem:
                handleRemoveRiddle(content, next.getUserID());
                break;
            case MessageType::Update:
                handleUpdateRiddle(content, next.getUserID());
                break;

        }
    }
    logger.write("Finish");
    mRunning.store(false);
}

void AdminModule::handleAddNewRiddle(ValueContent content, std::string user)
{
    if(!std::holds_alternative<std::pair<std::string, std::string>>(content))
    {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }

    auto riddlePair = std::get<std::pair<std::string, std::string>>(content);
    Riddle riddle(0, riddlePair.first, riddlePair.second);

    int result = adminService.addNewRiddle(riddle);

    looper->getDispatcher()->post(Message(MessageType::OK, user, serializer->serializeInt(result)));

}

void AdminModule::handleRemoveRiddle(ValueContent content, std::string user)
{
    if(!std::holds_alternative<int>(content))
    {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }

    int result = adminService.deleteRiddle(std::get<int>(content));

    looper->getDispatcher()->post(Message(MessageType::OK, user, serializer->serializeInt((result == 0 ? 1 : 0))));

}

void AdminModule::handleUpdateRiddle(ValueContent content, std::string user)
{
    if(!std::holds_alternative<std::tuple<int, std::string, std::string>>(content))
    {
        looper->getDispatcher()->post(Message(MessageType::Retransmit, user));
        return;
    }

    auto riddleTuple = std::get<std::tuple<int, std::string, std::string>>(content);
    Riddle riddle( std::get<0>(riddleTuple), std::get<1>(riddleTuple), std::get<2>(riddleTuple));

    int result = adminService.updateRiddle(riddle);

    looper->getDispatcher()->post(Message(MessageType::OK, user, serializer->serializeInt((result == 0 ? 1 : 0))));
}

void AdminModule::handleGetAllRiddles(std::string user)
{
    std::map<int, Riddle> riddles = adminService.getAllRiddlesFromBase();
    std::vector<std::tuple<int, std::string, std::string>> riddlesTuples;

    for(auto it = riddles.begin(); it != riddles.end(); ++it)
    {
        riddlesTuples.emplace_back(it->second.getId(), it->second.getRiddleContent(), it->second.getAnswer());
    }

    looper->getDispatcher()->post(Message(MessageType::Problems, user, serializer->serializeVector(riddlesTuples)));
}

AdminModule::~AdminModule()
{
    abortAndJoin();
}

bool AdminModule::run()
{
    try {
        mThread = std::thread(&AdminModule::runFunc, this);
    }
    catch(...)
    {
        return false;
    }

    return true;
}

bool AdminModule::running() const
{
    return mRunning.load();
}

void AdminModule::stop()
{
    abortAndJoin();
}

void AdminModule::abortAndJoin()
{
    mAbortRequested.store(true);
    if(mThread.joinable())
    {
        mThread.join();
    }
}

std::shared_ptr<AdminModule::Dispatcher> AdminModule::getDispatcher()
{
    return mDispatcher;
}

AdminModule::Dispatcher::Dispatcher(AdminModule &adminModule) : adminModule(adminModule){}

bool AdminModule::Dispatcher::post(Message &&message)
{
    return adminModule.post(std::move(message));
}

void AdminModule::setLooper(Looper *looper)
{
    this->looper = looper;
}