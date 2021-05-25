//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base) : base{user_base} {}

bool Authorization::authorize(User *user) {
    const CryptoPP::RSA::PublicKey& user_public_key = user->getPublicKey();

    CryptoPP::SecByteBlock key, iv;
    Cryptography::generate_symmetric_key(key, iv);

    // TODO set the user's symmetric key
    // TODO send symmetric key
}

bool Authorization::authorize(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        // TODO send error message
        return false;
    }

    return authorize(user);
}

bool Authorization::authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key) {
    User *user = base->getUser(username);

    if (user == nullptr) {
        base->addUser(User(username, std::move(public_key)));
    }
    return authorize(user);
}

void Authorization::setCurrentUser(const User &currentUser) {
    current_user = currentUser;
}

const User &Authorization::getCurrentUser() const {
    return current_user;
}
