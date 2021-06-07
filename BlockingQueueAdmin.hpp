#pragma once

#include<queue>
#include<mutex>
#include<condition_variable>
#include <atomic>

template<typename T>
class BlockingQueueAdmin {

private:
    std::queue<T> queue;

public:

    std::mutex admin;
    std::mutex server;

    void lockAdmin()
    {
        admin.lock();
        std::cout << "Lock admin!" << std::endl;
    }

    void lockServer()
    {
        server.lock();
        std::cout << "Lock server!" << std::endl;
    }

    void unlockAdmin()
    {
        admin.unlock();
        std::cout << "Unlock admin!" << std::endl;
    }

    void unlockServer()
    {
        server.unlock();
        std::cout << "Unlock server!" << std::endl;
    }

    void push(T const& _data){
        {
            queue.push(_data);
        }
    }

    bool empty() const{
        return queue.empty();
    }

    bool tryPop(T& _value){
        if (queue.empty())
        {
            return false;
        }

        _value = queue.front();
        queue.pop();
        return true;
    }

    void waitAndPop(T& _value){
        while (queue.empty())
        {}

        _value = queue.front();
        queue.pop();
    }

    bool tryWaitAndPop(T& _value, int _milli){
        while (queue.empty())
        {
            if (queue.empty()) {
                return false;
            }
        }

        _value = queue.front();
        queue.pop();
        return true;
    }
};

