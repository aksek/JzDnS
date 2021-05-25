//author: Patryk Karbownik

#ifndef JZDNS_USER_H
#define JZDNS_USER_H


#include <string>
#include <cryptopp/rsa.h>
#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>

class User {
private:
    std::string nickName;
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;

public:
    User(std::string nickName);
    User(std::string nickName, CryptoPP::RSA::PublicKey);
    const std::string &getNickName() const;
    void setNickName(const std::string &nickName);
    const CryptoPP::RSA::PublicKey &getPublicKey() const;
    void setPublicKey(const CryptoPP::RSA::PublicKey &publicKey);
    const CryptoPP::SecByteBlock &getKey() const;
    void setKey(const CryptoPP::SecByteBlock &key);
    const CryptoPP::SecByteBlock &getIv() const;
    void setIv(const CryptoPP::SecByteBlock &iv);
};


#endif //JZDNS_USER_H
