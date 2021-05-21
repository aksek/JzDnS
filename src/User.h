//author: Patryk Karbownik

#ifndef JZDNS_USER_H
#define JZDNS_USER_H


#include <string>
#include <cryptopp/rsa.h>

class User {
private:
    std::string login;
    CryptoPP::RSA::PublicKey publicKey;

public:
    User(std::string login, CryptoPP::RSA::PublicKey);
    const std::string &getLogin() const;
    void setLogin(const std::string &login);
    const CryptoPP::RSA::PublicKey &getPublicKey() const;
    void setPublicKey(const CryptoPP::RSA::PublicKey &publicKey);
};


#endif //JZDNS_USER_H
