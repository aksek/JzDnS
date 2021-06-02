//
// Created by aniela on 5/22/21.
//
#pragma once

#include "message.h"
#include "authorization.hpp"
#include "RiddleService.h"
#include "AdminService.h"

class Dispatcher {
    Message* lastMessage;

    Authorization* authorization;
    RiddleService* riddleService;
    AdminService* adminService;
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
    explicit Dispatcher(Authorization* authorization, RiddleService* riddleService, AdminService* adminService, SerializeContent* serializeContent);

    Message dispatch(Message message);
};


