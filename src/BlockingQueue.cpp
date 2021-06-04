//
// Created by aniela on 6/3/21.
//

//#include "BlockingQueue.hpp"

template<typename T>
void BlockingQueue<T>::push(const T &_data) {
    {
        std::lock_guard<std::mutex> lock(guard);
        queue.push(_data);
    }
    signal.notify_one();
}

template<typename T>
bool BlockingQueue<T>::empty() const {
    std::lock_guard<std::mutex> lock(guard);
    return queue.empty();
}

template<typename T>
bool BlockingQueue<T>::tryPop(T &_value) {
    std::lock_guard<std::mutex> lock(guard);
    if (queue.empty())
    {
        return false;
    }

    _value = queue.front();
    queue.pop();
    return true;
}

template<typename T>
void BlockingQueue<T>::waitAndPop(T &_value) {
    std::unique_lock<std::mutex> lock(guard);
    while (queue.empty())
    {
        signal.wait(lock);
    }

    _value = queue.front();
    queue.pop();
}

template<typename T>
bool BlockingQueue<T>::tryWaitAndPop(T &_value, int _milli) {
    std::unique_lock<std::mutex> lock(guard);
    while (queue.empty())
    {
        signal.wait_for(lock, std::chrono::milliseconds(_milli));
        return false;
    }

    _value = queue.front();
    queue.pop();
    return true;
}
