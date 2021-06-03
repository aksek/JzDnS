//
// Created by aniela on 5/22/21.
//
#pragma once

#include "message.h"
#include "authorization.hpp"
#include "RiddleService.h"
#include "AdminService.h"
#include "BlockingQueue.hpp"

typedef BlockingQueue<Message> MessageQueue;

class Dispatcher {
    Message* lastMessage;

    MessageQueue *authorizationQueue;
    MessageQueue *riddleServiceQueue;
    MessageQueue *adminServiceQueue;
    SerializeContent* serializer;

    Message handleRetransmit(ValueContent content);
    Message handleLogin(ValueContent content);
    Message handleRegister(ValueContent content);
    Message handleSolution(ValueContent content);
    Message handleNewProblem(ValueContent content);
    Message handleDeleteProblem(ValueContent content);
    Message handleEditProblem(ValueContent content);
    Message handleEditSolution(ValueContent content);

public:
    explicit Dispatcher(MessageQueue* authorization, MessageQueue* riddleService, MessageQueue* adminService, SerializeContent* serializeContent);

    Message dispatch(Message message);
};


