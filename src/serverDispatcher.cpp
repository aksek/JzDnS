//
// Created by aniela on 5/22/21.
//

#include "serverDispatcher.hpp"


Dispatcher::Dispatcher(Authorization* authorization, RiddleService* riddleService, AdminService* adminService, SerializeContent* serializeContent)
    : authorization{authorization},
      riddleService{riddleService},
      adminService{adminService},
      serializer{serializeContent} {}

Message Dispatcher::dispatch(Message message) {
    MessageType type = message.getMessageType();
    ValueContent content = serializer->deserialize(type, message.getContent());

    switch (type) {
        case MessageType::Retransmit:
            return handleRetransmit(content);
        case MessageType::Register:
            return handleRegister(content);
        case MessageType::Login:
            return handleLogin(content);
        case MessageType::Solution:
            return handleSolution(content);
        case MessageType::New_problem:
            return handleNewProblem(content);
        case MessageType::Delete_problem:
            return handleDeleteProblem(content);
        case MessageType::Edit_problem:
            return handleEditProblem(content);
        case MessageType::Edit_solution:
            return handleEditSolution(content);
        default:
            return Message(MessageType::Retransmit);

    }
}

Message Dispatcher::handleRetransmit(ValueContent content) {
    if (lastMessage != nullptr) {
        return *lastMessage;
    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleLogin(ValueContent content) {
    if (std::holds_alternative<std::string>(content)) {
        std::string username = std::get<std::string>(content);
        Symmetric_key_pair key_pair = authorization->authorize(username);
        auto responseContent = serializer->serializeKey(key_pair);
        return Message(MessageType::Key, responseContent);

    } else return Message(MessageType::Retransmit);
}


Message Dispatcher::handleRegister(ValueContent content) {
    if (std::holds_alternative<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content)) {
        auto username_key = std::get<std::pair<std::string, CryptoPP::RSA::PublicKey> >(content);
        Symmetric_key_pair key_pair = authorization->authorize(username_key.first, username_key.second);
        auto responseContent = serializer->serializeKey(key_pair);

        return Message(MessageType::Key, responseContent);
    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleSolution(ValueContent content) {
    if (std::holds_alternative<std::string>(content)) {
        std::string answer = std::get<std::string>(content);

        Riddle riddle = riddleService->getLastRiddle();
        std::string correct_answer = riddle.getAnswer();

        bool correct = (answer == correct_answer);
        auto responseContent = serializer->serializeBool(correct);

        return Message(MessageType::Correct, responseContent);

    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleNewProblem(ValueContent content) {
    if (authorization->getCurrentUser()->getUserType() != User::UserType::ADMIN) {
        return Message(MessageType::Retransmit);
    }

    if (std::holds_alternative<std::pair<std::string, std::string> >(content)) {
        auto problem_solution = std::get<std::pair<std::string, std::string> >(content);

        Riddle riddle(0, problem_solution.first, problem_solution.second);

        int id = adminService->addNewRiddle(riddle);
        if (id == -1) {
            return Message(MessageType::Retransmit);
        }

        auto responseContent = serializer->serializeInt(id);
        return Message(MessageType::OK, responseContent);

    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleDeleteProblem(ValueContent content) {
    if (authorization->getCurrentUser()->getUserType() != User::UserType::ADMIN) {
        return Message(MessageType::Retransmit);
    }
    if (std::holds_alternative<int>(content)) {

        int id = std::get<int>(content);

        if (adminService->deleteRiddle(id) != 0) {
            return Message(MessageType::Retransmit);
        }

        return Message(MessageType::OK);
    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleEditProblem(ValueContent content) {
    if (authorization->getCurrentUser()->getUserType() != User::UserType::ADMIN) {
        return Message(MessageType::Retransmit);
    }
    if (std::holds_alternative<std::pair<int, std::string> >(content)) {

        auto id_problem = std::get<std::pair<int, std::string> > (content);

        Riddle riddle = adminService.getRiddle(id_problem.first);
        riddle.setRiddleContent(id_problem.second);
        if (adminService->updateRiddle(riddle) != 0) {
            return Message(MessageType::Retransmit);
        }

        return Message(MessageType::OK);
    } else return Message(MessageType::Retransmit);
}

Message Dispatcher::handleEditSolution(ValueContent content) {
    if (authorization->getCurrentUser()->getUserType() != User::UserType::ADMIN) {
        return Message(MessageType::Retransmit);
    }
    if (std::holds_alternative<std::pair<int, std::string> >(content)) {

        auto id_solution = std::get<std::pair<int, std::string> > (content);

        Riddle riddle = adminService.getRiddle(id_solution.first);
        riddle.setAnswer(id_solution.second);
        if (adminService->updateRiddle(riddle) != 0) {
            return Message(MessageType::Retransmit);
        }

        return Message(MessageType::OK);
    } else return Message(MessageType::Retransmit);
}

