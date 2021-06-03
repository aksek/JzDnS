//
// Created by aniela on 5/22/21.
//
#pragma once

#include "User.h"
#include "UserBase.h"

typedef std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> Symmetric_key_pair;


class Authorization {
private:
    User* current_user;

    UserBase* base;

public:
    explicit Authorization(UserBase* user_base);
    CryptoPP::RSA::PublicKey authorize(std::string username);
    CryptoPP::RSA::PublicKey authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key);

    [[nodiscard]] const User* getCurrentUser() const;
};

