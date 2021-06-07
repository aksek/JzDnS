//
// Created by aniela on 6/3/21.
//
#pragma once

#include<queue>
#include<mutex>
#include<condition_variable>
#include <atomic>
template<typename T>
class BlockingQueue {

private:
    const int limit = 10;
    std::queue<T> queue;
    mutable std::mutex guard;
    std::condition_variable empty;
    std::condition_variable full;

public:
    void push(T const& _data){
        {
            std::unique_lock<std::mutex> lock(guard);
            while (queue.size() >= limit) {
                full.wait(lock);
            }

            queue.push(_data);
        }
        empty.notify_one();
    }

    // bool empty() const{
    //     std::lock_guard<std::mutex> lock(guard);
    //     return queue.empty();
    // }

    // bool full() const {
    //     std::lock_guard<std::mutex> lock(guard);
    //     return queue.size() >= limit;
    // }

    bool tryPop(T& _value){
        {
            std::lock_guard<std::mutex> lock(guard);
            if (queue.empty())
            {
                return false;
            }

            _value = queue.front();
            queue.pop();
        }

        full.notify_one();
        return true;
    }

    void waitAndPop(T& _value){
        {
            std::unique_lock<std::mutex> lock(guard);
            while (queue.empty())
            {
                empty.wait(lock);
            }
            _value = queue.front();
            queue.pop();
        }
        full.notify_one();
    }

    bool tryWaitAndPop(T& _value, int _milli){
        {
            std::unique_lock<std::mutex> lock(guard);
            while (queue.empty())
            {
                empty.wait_for(lock, std::chrono::milliseconds(_milli));
                if (queue.empty()) {
                    return false;
                }

            }
            _value = queue.front();
            queue.pop();
        }
        full.notify_one();
        return true;
    }
};

