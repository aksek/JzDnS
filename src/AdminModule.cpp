//
// Created by patryk on 03.06.2021.
//

#include "AdminModule.h"


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

AdminModule::AdminModule(RiddleBase* riddleBase, Looper* looper) :
mDispatcher(std::shared_ptr<Dispatcher>(new Dispatcher(*this))),
mRunning(false),
mAbortRequested(false),
messagesQueue(),
looper(looper),
adminService(riddleBase),
serializer()
{}

void AdminModule::runFunc()
{
    mRunning.store(true);

    while (!mAbortRequested.load())
    {
        Message next(MessageType::OK, "");
        messagesQueue.waitAndPop(next);

        MessageType type = next.getMessageType();
        ValueContent content = serializer->deserialize(type, next.getContent());

        switch (type) {
            case MessageType::Problems:
                break;
            case MessageType::New_problem:
                //sprawdz czy jest dobrego typu
                //utworz zagadke
                //przekaz do handlera
                //zrob to samo
                break;
            case MessageType::Delete_problem:
                break;
            case MessageType::Edit_problem:
                break;
            case MessageType::Edit_solution:
                break;

        }
    }
}

void AdminModule::handleAddNewRiddle()
{
    int addNewRiddle(Riddle riddle);
}

void AdminModule::handleRemoveRiddle()
{

}

void AdminModule::handleUpdateRiddle()
{

}

void AdminModule::handleGetAllRiddles()
{

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