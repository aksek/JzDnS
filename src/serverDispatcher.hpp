//
// Created by aniela on 5/22/21.
//
#pragma once

#include "message.h"
#include "authorization.hpp"

class Dispatcher {
    Authorization* authorization;
//    RiddleService* riddleService;

public:
    explicit Dispatcher(Authorization* authorization/*, RiddleService* riddleService*/);

    void dispatch(Message message);
};


