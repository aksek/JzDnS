//
// Created by aniela on 6/4/21.
//

#ifndef JZDNS_QUEUEMAP_HPP
#define JZDNS_QUEUEMAP_HPP


#include <map>
#include "BlockingQueue.hpp"
#include "message.h"
#include "authorization.hpp"

class QueueMap {
    std::map<std::string, BlockingQueue<Message>* > queues;
    std::mutex guard;
    Authorization* authorization;
public:

    void setAuthorization(Authorization *authorization);
    void post_to(const std::string& user, Message message);
    void post_except(const std::string& user, Message message);
    void add_user(const std::string& user, BlockingQueue<Message>* queue);
    Message pop(std::string user);
    bool isInMap(std::string userName);
};


#endif //JZDNS_QUEUEMAP_HPP
