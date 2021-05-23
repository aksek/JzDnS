//Author: Patryk Karbownik

#include "UserBase.h"

int UserBase::addUser(User user)
{
    auto it = users.find(user.getLogin());
    if(it == users.end())
    {
        users.insert(std::pair<std::string, User>(user.getLogin(), user));
        return 0;
    }
    return -1;
}

int UserBase::removeUser(User user)
{
    auto it = users.find(user.getLogin());

    if(it != users.end())
    {
        users.erase(it);
        return 0;
    }
    return -1;
}

User* UserBase::getUser(std::string login)
{
    auto iter = users.find(login);

    if(iter != std::end(users))
    {
        User* u = &iter->second;
        return u;
    }

    return nullptr;
}

int UserBase::updateUser(User user)
{
    auto iter = users.find(user.getLogin());

    if(iter != std::end(users))
    {
        iter->second.setPublicKey(user.getPublicKey());
        return 0;
    }

    return -1;
}
/*
void UserBase::loadBaseFromDisk(std::string path)
{
    std::ifstream in(path);

    boost::archive::text_iarchive i_archive(in);

    i_archive >> users;
}

void UserBase::saveBaseOnDisk(std::string path)
{
    std::ofstream ofs(path);

    boost::archive::text_oarchive oa(ofs);

    oa << users;
}*/