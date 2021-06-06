//
// Created by patryk on 03.06.2021.
//

#ifndef JZDNS_ADMINMODULE_H
#define JZDNS_ADMINMODULE_H


#include <thread>
#include <atomic>
#include "BlockingQueue.hpp"
//#include "Looper.hpp"
#include <map>

class Looper;

class AdminModule {
private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

    BlockingQueue<Message> messagesQueue;

    AdminService adminService;
    SerializeContent* serializer;
    Looper* looper;

    void handleAddNewRiddle(ValueContent content, std::string user);
    void handleRemoveRiddle(ValueContent content, std::string user);
    void handleUpdateRiddle(ValueContent content, std::string user);
    void handleGetAllRiddles(std::string user);

    void runFunc();
    bool post(Message &&aMessage);
public:
    class Dispatcher {
        friend class AdminModule;
    private:
        AdminModule &adminModule;
        explicit Dispatcher(AdminModule &adminModule);
    public:
        bool post(Message &&message);
    };

    AdminModule(RiddleBase* riddleBase);
    void setLooper(Looper* looper);
    ~AdminModule();

    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();
    std::shared_ptr<Dispatcher> getDispatcher();

private:
    std::shared_ptr<Dispatcher> mDispatcher;
};


#endif //JZDNS_ADMINMODULE_H
