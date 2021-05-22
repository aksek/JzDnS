//
// Created by aniela on 5/22/21.
//
#pragma once

#include "message.h"
#include "authorization.hpp"

class Dispatcher {
    Authorization* authorization;

public:
    Dispatcher(Authorization* authorization);

    void dispatch(Message message);
};


