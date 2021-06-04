//
// Created by aniela on 6/4/21.
//

#include "QueueMap.hpp"

void QueueMap::post_to(const std::string& user, Message message) {
    queues[user]->push(message);
}

void QueueMap::post_except(const std::string& user, Message message) {
    guard.lock();

    for (auto user_queue : queues) {
        if (user_queue.first != user) {
            user_queue.second->push(message);
        }
    }

    guard.unlock();
}

void QueueMap::add_user(const std::string& user, BlockingQueue<Message> *queue) {
    guard.lock();

    queues.insert(std::make_pair(user, queue));

    guard.unlock();
}

Message QueueMap::pop(std::string user) {
    Message message(MessageType::OK, "");
    queues[user]->waitAndPop(message);
    return message;
}

