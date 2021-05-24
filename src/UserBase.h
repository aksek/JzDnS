//Author: Patryk Karbownik

#ifndef JZDNS_USERBASE_H
#define JZDNS_USERBASE_H


#include "User.h"

#include <fstream>

class UserBase {
private:
    std::map<std::string, User> users;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(users);
    }
public:
    int addUser(User user);
    int removeUser(User user);
    User* getUser(std::string login);
    int updateUser(User user);
};


#endif //JZDNS_USERBASE_H
