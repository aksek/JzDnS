//Author: Patryk Karbownik

#ifndef JZDNS_USERBASE_H
#define JZDNS_USERBASE_H


#include "User.h"
#include "boost/serialization/map.hpp"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <mutex>
#include "../cryptography.hpp"

class UserBase {
private:
    std::map<std::string, User> users;
    std::mutex m;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(users);
    }
public:
    int addUser(User user);
    int removeUser(std::string nickName);
    User* getUser(std::string nickName);
    int updateUser(User user);
    void addAdmin(std::string userName, std::string path);
    void saveBaseOnDisk(std::string basicString);
    void loadBaseFromDisk(std::string path);
};


#endif //JZDNS_USERBASE_H
