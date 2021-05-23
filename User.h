//author: Patryk Karbownik

#ifndef JZDNS_USER_H
#define JZDNS_USER_H


#include <string>
#include <cryptopp/rsa.h>
#include <boost/serialization/access.hpp>

class User {
private:
    std::string login;
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::SecByteBlock key;

private:
    CryptoPP::SecByteBlock iv;

public:
    User(std::string login, CryptoPP::RSA::PublicKey);
    const std::string &getLogin() const;
    void setLogin(const std::string &login);
    const CryptoPP::RSA::PublicKey &getPublicKey() const;
    void setPublicKey(const CryptoPP::RSA::PublicKey &publicKey);
    const CryptoPP::SecByteBlock &getKey() const;
    void setKey(const CryptoPP::SecByteBlock &key);
    const CryptoPP::SecByteBlock &getIv() const;
    void setIv(const CryptoPP::SecByteBlock &iv);
};


#endif //JZDNS_USER_H
