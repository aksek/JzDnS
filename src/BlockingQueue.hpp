//
// Created by aniela on 6/3/21.
//
#pragma once

#include<queue>
#include<mutex>
#include<condition_variable>

template<typename T>
class BlockingQueue {

private:
    std::queue<T> queue;
    mutable std::mutex guard;
    std::condition_variable signal;

public:
    void push(T const& _data);

    bool empty() const;

    bool tryPop(T& _value);

    void waitAndPop(T& _value);

    bool tryWaitAndPop(T& _value, int _milli);
};

