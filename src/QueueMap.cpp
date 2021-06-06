//
// Created by aniela on 6/4/21.
//

#include "cryptography.hpp"
#include "QueueMap.hpp"

void QueueMap::post_to(const std::string& user, Message message) {

    CryptoPP::AutoSeededRandomPool rng;
    CryptoPP::RSA::PublicKey user_key = authorization->getKey(user);
    std::string encrypted = Cryptography::asymmetric_encrypt(user_key, message.getContentText(), rng);
    Message encrypted_message(message.getMessageType(), user, encrypted, encrypted.size());
    queues[user]->push(encrypted_message);
}

void QueueMap::post_except(const std::string& user, Message message) {
    CryptoPP::AutoSeededRandomPool rng;
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
    Message message(MessageType::Login_error, "");
    queues[user]->waitAndPop(message);

    return message;
}

QueueMap::QueueMap(Authorization *authorization1) : authorization(authorization1){}

