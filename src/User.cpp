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

const CryptoPP::SecByteBlock &User::getKey() const {
    return key;
}

void User::setKey(const CryptoPP::SecByteBlock &key) {
    User::key = key;
}

const CryptoPP::SecByteBlock &User::getIv() const {
    return iv;
}

void User::setIv(const CryptoPP::SecByteBlock &iv) {
    User::iv = iv;
}
