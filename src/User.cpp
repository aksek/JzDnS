//author: Patryk Karbownik

#include "User.h"

User::User(std::string nickName, UserType type)
{
    this->nickName = nickName;
    this->type = type;
}

User::User(std::string nickName, UserType type, CryptoPP::RSA::PublicKey publicKey){
    User::nickName = nickName;
    User::type = type;
    User::publicKey = publicKey;
}

const std::string &User::getNickName() const {
    return nickName;
}

void User::setNickName(const std::string &nickName) {
    User::nickName = nickName;
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

const User::UserType User::getUserType() const
{
    return type;
}

bool User::operator==(const User &rhs) const {
    std::string temp;
    CryptoPP::StringSink ss(temp);
    publicKey.Save(ss);

    std::string temp2;
    CryptoPP::StringSink ss2(temp2);
    rhs.publicKey.Save(ss2);

    return nickName == rhs.nickName &&
           temp == temp2 &&
           type == rhs.type;
}

bool User::operator!=(const User &rhs) const {
    return !(rhs == *this);
}

User::User() {}
