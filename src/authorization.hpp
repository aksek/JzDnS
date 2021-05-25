//
// Created by aniela on 5/22/21.
//
#pragma once

#include "User.h"
#include "UserBase.h"

class Authorization {
    User current_user;
public:
    const User &getCurrentUser() const;

public:
    void setCurrentUser(const User &currentUser);

private:
    UserBase* base;
    static bool authorize(User* user);
public:
    explicit Authorization(UserBase* user_base);
    bool authorize(std::string username);
    bool authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key);

};

