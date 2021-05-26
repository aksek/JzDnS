//
// Created by aniela on 5/22/21.
//

#include "serverDispatcher.hpp"


Dispatcher::Dispatcher(Authorization* authorization, RiddleService* riddleService, SerializeContent* serializeContent)
    : authorization{authorization},
      riddleService{riddleService},
      serializer{serializeContent} {}

void Dispatcher::dispatch(Message message) {
        MessageType type = message.getMessageType();
        ValueContent content = serializer->deserialize(type, message.getContent());

//        authorization->setCurrentUser(message.getUser());


    switch (type) {
        case MessageType::Retransmit:
            handleRetransmit(content);
            break;
        case MessageType::Login:
            handleLogin(content);
            break;
        case MessageType::Solution:
            handleSolution(content);
            break;
        case MessageType::New_problem:
            handleNewProblem(content);
            break;
        case MessageType::Delete_problem:
            handleDeleteProblem(content);
            break;
        case MessageType::Edit_problem:
            handleEditProblem(content);
            break;
        default:
            break;

    }
}

void Dispatcher::handleRetransmit(ValueContent content) {
    if (lastMessage != nullptr) {
        //TODO send last message
    }
}

void Dispatcher::handleLogin(ValueContent content) {
    if (std::holds_alternative<std::string>(content)) {
        std::string username = std::get<std::string>(content);
        Symmetric_key_pair key_pair = authorization->authorize(username);
    }
//            } else if (std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
//                auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);
//                authorization->authorize(username_key.first(), username_key.second());
//            }
    // TODO send symmetric key
}

void Dispatcher::handleSolution(ValueContent content) {
    if (std::holds_alternative<std::string>(content)) {
        std::string answer = std::get<std::string>(content);

        bool correct = riddleService->checkAnswer(answer);
        std::pair<std::string, size_t> responseContent = serializer->serializeBool(correct);

        Message response(MessageType::Correct, responseContent);
        // TODO send response
    }
}

void Dispatcher::handleNewProblem(ValueContent content) {

}

void Dispatcher::handleDeleteProblem(ValueContent content) {

}

void Dispatcher::handleEditProblem(ValueContent content) {

}

