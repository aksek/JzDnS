//
// Created by aniela on 5/22/21.
//

#include "serverDispatcher.hpp"


Dispatcher::Dispatcher(Authorization* authorization/*, RiddleService riddleService*/)
    : authorization{authorization}/*, riddleService{riddleService}*/ {}

void Dispatcher::dispatch(Message message) {
//    authorization->setCurrentUser(message.getUser());

    Messages messages = Messages();
    ValueTextMessage content = messages.getText(&message);    //variant

    switch (message.getMessageType()) {
        case MessageType::Retransmit:
            break;
        case MessageType::Login:
            if (std::holds_alternative<std::string>(content)) {
                std::string username = std::get<std::string>(content);
                authorization->authorize(username);
            }
//            } else if (std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
//                auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);
//                authorization->authorize(username_key.first(), username_key.second());
//            }
            break;
        case MessageType::Solution:
            if (std::holds_alternative<std::string>(content)) {
                std::string answer = std::get<std::string>(content);
                size_t size = 207;
                DynamicJsonDocument doc(size);
                doc["header"]["type"] = "Correct";
                doc["header"]["size"] = size;
                doc["header"]["control"] = 0;
                if (riddleService.checkAnswer(answer)) {
                    doc["text"]["correct"] = true;
                } else {
                    doc["text"]["correct"] = false;
                }
                MessageBool response(MessageType::Correct, doc);

                // TODO send response
            }
            break;
        case MessageType::New_problem:
            break;
        case MessageType::Delete_problem:
            break;
        case MessageType::Edit_problem:
            break;
        case MessageType::Problem:
            break;
        case MessageType::Correct:
            break;
        case MessageType::Round_over:
            break;
        case MessageType::Problems:
            break;
        default:
            break;

    }
}

