//Author: Patryk Karbownik

#ifndef JZDNS_USERBASE_H
#define JZDNS_USERBASE_H


#include "User.h"

#include <fstream>

class UserBase {
private:
    std::map<std::string, User> users;

public:
    int addUser(User user);
    int removeUser(std::string nickName);
    User* getUser(std::string nickName);
    int updateUser(User user);
};


#endif //JZDNS_USERBASE_H
