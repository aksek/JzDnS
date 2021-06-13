//
// Created by patryk on 04.06.2021.
//

#ifndef JZDNS_RIDDLEMODULE_H
#define JZDNS_RIDDLEMODULE_H


#include <thread>
#include <atomic>
#include "../message.h"
#include "../BlockingQueue.hpp"
#include "RiddleService.h"
//#include "Looper.hpp"
class Looper;

class RiddleModule {
private:
    std::thread mThread;
    std::atomic_bool mRunning;
    std::atomic_bool mAbortRequested{};

    BlockingQueue<Message> messagesQueue;

    RiddleService riddleService;
    SerializeContent* serializer;
    Looper* looper;

    void handleGetSolution(ValueContent content, std::string user);
    void handleGetProblem(std::string user);

    void runFunc();
    bool post(Message &&aMessage);
public:
    class Dispatcher {
        friend class RiddleModule;
    private:
        RiddleModule &riddleModule;
        explicit Dispatcher(RiddleModule &riddleModule);
    public:
        bool post(Message &&message);
    };

    RiddleModule(RiddleBase* riddleBase);
    ~RiddleModule();

    void setLooper(Looper* looper);
    bool run();
    bool running() const;
    void stop();
    void abortAndJoin();
    std::shared_ptr<Dispatcher> getDispatcher();
private:
    std::shared_ptr<Dispatcher> mDispatcher;
};


#endif //JZDNS_RIDDLEMODULE_H
