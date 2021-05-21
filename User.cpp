//author: Patryk Karbownik

#include "User.h"

User::User(std::string login, CryptoPP::RSA::PublicKey publicKey){
    User::login = login;
    User::publicKey = publicKey;
}

const std::string &User::getLogin() const {
    return login;
}

void User::setLogin(const std::string &login) {
    User::login = login;
}

const CryptoPP::RSA::PublicKey &User::getPublicKey() const {
    return publicKey;
}

void User::setPublicKey(const CryptoPP::RSA::PublicKey &publicKey) {
    User::publicKey = publicKey;
}
