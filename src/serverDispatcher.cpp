//
// Created by aniela on 5/22/21.
//

#include "serverDispatcher.hpp"


Dispatcher::Dispatcher(Authorization* authorization) : authorization{authorization} {}

void Dispatcher::dispatch(Message message) {
    auto content = message.getContent();    //variant

    switch (message.getMessageType()) {
        case MessageType::Retransmit:
            break;
        case MessageType::Login:
            if (std::holds_alternative<std::string>(content)) {
                std::string username = std::get<std::string>(content);
                authorization->authorize(username);

            } else if (std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
                auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);
                authorization->authorize(username_key.first(), username_key.second());
            }
            break;
        case MessageType::Solution:
            break;
        case MessageType::New_problem:
            break;
        case MessageType::Delete_problem:
            break;
        case MessageType::Edit_problem:
            break;
        default:
            break;

    }
}

