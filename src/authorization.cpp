//
// Created by aniela on 5/22/21.
//

#include "authorization.hpp"
#include "cryptography.hpp"

#include <utility>


Authorization::Authorization(UserBase *user_base) : base{user_base} {}

Symmetric_key_pair Authorization::authorize(User *user) {
    const CryptoPP::RSA::PublicKey& user_public_key = user->getPublicKey();

    CryptoPP::SecByteBlock key, iv;
    Cryptography::generate_symmetric_key(key, iv);

    current_user = user;

    // TODO set the user's symmetric key
    return std::make_pair(key, iv);
}

Symmetric_key_pair Authorization::authorize(std::string username) {
    User *user = base->getUser(std::move(username));

    if (user == nullptr) {
        return std::make_pair(CryptoPP::SecByteBlock(), CryptoPP::SecByteBlock());
    }

    return authorize(user);
}

Symmetric_key_pair Authorization::authorize(const std::string& username, CryptoPP::RSA::PublicKey public_key) {
    User *user = base->getUser(username);

    if (user == nullptr) {
        base->addUser(User(username, std::move(public_key)));
    }
    return authorize(user);
}

const User* Authorization::getCurrentUser() const {
    return current_user;
}
