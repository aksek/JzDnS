#pragma once

#include<queue>
#include<mutex>
#include<condition_variable>
#include <atomic>

template<typename T>
class BlockingQueueAdmin {

private:
    std::queue<T> queue;
    std::mutex admin;
    std::mutex server;

public:

    void lockAdmin()
    {
        admin.lock();
    }

    void lockServer()
    {
        server.lock();
    }

    void unlockAdmin()
    {
        admin.unlock();
    }

    void unlockServer()
    {
        server.unlock();
    }

    void push(T const& _data){
        {
            queue.push(_data);
        }
    }

    bool empty() const{
        return queue.empty();
    }

    void waitAndPop(T& _value){
        while (queue.empty())
        {}

        _value = queue.front();
        queue.pop();
    }


};

