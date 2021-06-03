//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base) : base{user_base} {}

CryptoPP::RSA::PublicKey Authorization::authorize(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        return CryptoPP::RSA::PublicKey();
    }

    return user->getPublicKey();
}

CryptoPP::RSA::PublicKey Authorization::authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key) {
    User *user = base->getUser(username);

    if (user == nullptr) {
        if (base->addUser(User(username, User::UserType::NORMAL, public_key)) == 0) {
            return public_key;
        } else return CryptoPP::RSA::PublicKey();
    }
    return user->getPublicKey();
}

const User* Authorization::getCurrentUser() const {
    return current_user;
}
