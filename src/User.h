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
    UserType type;

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar) const
    {
        ar & nickName;
        ar & type;
        CryptoPP::SavePublicKey("/keys/" + nickName, publicKey);
    }

    template<class Archive>
    void load(Archive & ar)
    {
        ar & nickName;
        ar & type;
        CryptoPP::LoadPublicKey("/keys/" + nickName,  publicKey);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    enum UserType {ADMIN, NORMAL};
    User(std::string nickName, UserType type);
    User(std::string nickName, UserType type, CryptoPP::RSA::PublicKey);
    const std::string &getNickName() const;
    void setNickName(const std::string &nickName);
    const CryptoPP::RSA::PublicKey &getPublicKey() const;
    void setPublicKey(const CryptoPP::RSA::PublicKey &publicKey);
    const CryptoPP::SecByteBlock &getKey() const;
    void setKey(const CryptoPP::SecByteBlock &key);
    const CryptoPP::SecByteBlock &getIv() const;
    void setIv(const CryptoPP::SecByteBlock &iv);
    const UsertType getUserType() const;
};


#endif //JZDNS_USER_H
