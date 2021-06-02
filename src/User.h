//author: Patryk Karbownik

#ifndef JZDNS_USER_H
#define JZDNS_USER_H


#include <string>
#include <cryptopp/rsa.h>
#include <boost/serialization/split_member.hpp>

class User {
public:
    enum UserType {ADMIN, NORMAL};
private:
    std::string nickName;
    CryptoPP::RSA::PublicKey publicKey;
    CryptoPP::SecByteBlock key;
    CryptoPP::SecByteBlock iv;
    UserType type;

    friend class boost::serialization::access;
    template<class Archive>
    void save(Archive & ar, const unsigned int version) const
    {
        ar & nickName;
        ar & type;
        std::string temp;
        CryptoPP::StringSink ss(temp);
        publicKey.Save(ss);
        ar & temp;
    }

    template<class Archive>
    void load(Archive & ar, const unsigned int version)
    {
        ar & nickName;
        ar & type;
        std::string temp;
        ar & temp;
        CryptoPP::StringSource ss(temp, true);
        publicKey.Load(ss);
    }
    BOOST_SERIALIZATION_SPLIT_MEMBER()
public:
    User();

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
    const UserType getUserType() const;
    bool operator==(const User &rhs) const;
    bool operator!=(const User &rhs) const;
};


#endif //JZDNS_USER_H
