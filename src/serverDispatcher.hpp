//
// Created by aniela on 5/22/21.
//
#pragma once

#include "message.h"
#include "authorization.hpp"
#include "RiddleService.h"

class Dispatcher {
    Message* lastMessage;

    Authorization* authorization;
    RiddleService* riddleService;
    SerializeContent* serializer;

    void handleRetransmit(ValueContent content);
    void handleLogin(ValueContent content);
    void handleSolution(ValueContent content);
    void handleNewProblem(ValueContent content);
    void handleDeleteProblem(ValueContent content);
    void handleEditProblem(ValueContent content);

public:
    explicit Dispatcher(Authorization* authorization, RiddleService* riddleService, SerializeContent* serializeContent);

    void dispatch(Message message);
};


