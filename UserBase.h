//Author: Patryk Karbownik

#ifndef JZDNS_USERBASE_H
#define JZDNS_USERBASE_H


#include <map>
#include <cryptopp/rsa.h>
#include "User.h"
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <fstream>

class UserBase {
private:
    std::map<std::string, User> users;
public:
    int addUser(User user);
    int removeUser(User user);
    User* getUser(std::string login);
    int updateUser(User riddle);
    void loadBaseFromDisk(std::string path);
    void saveBaseOnDisk(std::string path);
};


#endif //JZDNS_USERBASE_H
