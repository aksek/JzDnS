//Author: Patryk Karbownik

#include "UserBase.h"

int UserBase::addUser(User user)
{
    m.lock();
    auto it = users.find(user.getNickName());
    if(it == users.end())
    {
        users.insert(std::pair<std::string, User>(user.getNickName(), user));
        m.unlock();
        return 0;
    }
    m.unlock();
    return -1;
}

int UserBase::removeUser(std::string nickName)
{
    m.lock();
    auto it = users.find(nickName);

    if(it != users.end())
    {
        users.erase(it);
        m.unlock();
        return 0;
    }
    m.unlock();
    return -1;
}

User* UserBase::getUser(std::string nickName)
{
    m.lock();
    auto iter = users.find(nickName);

    if(iter != std::end(users))
    {
        User* u = &iter->second;
        m.unlock();
        return u;
    }
    m.unlock();
    return nullptr;
}

int UserBase::updateUser(User user)
{
    m.lock();
    auto iter = users.find(user.getNickName());

    if(iter != std::end(users))
    {
        iter->second.setPublicKey(user.getPublicKey());
        m.unlock();
        return 0;
    }
    m.unlock();
    return -1;
}

void UserBase::loadBaseFromDisk(std::string path)
{
    m.lock();
    std::ifstream in(path);

    boost::archive::text_iarchive i_archive(in);

    i_archive >> users;
    m.unlock();
}

void UserBase::saveBaseOnDisk(std::string path)
{
    m.lock();
    std::ofstream ofs(path);

    boost::archive::text_oarchive oa(ofs);

    oa << users;
    m.unlock();
}