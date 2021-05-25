//Author: Patryk Karbownik

#include "UserBase.h"

int UserBase::addUser(User user)
{
    auto it = users.find(user.getNickName());
    if(it == users.end())
    {
        users.insert(std::pair<std::string, User>(user.getNickName(), user));
        return 0;
    }
    return -1;
}

int UserBase::removeUser(std::string nickName)
{
    auto it = users.find(nickName);

    if(it != users.end())
    {
        users.erase(it);
        return 0;
    }
    return -1;
}

User* UserBase::getUser(std::string nickName)
{
    auto iter = users.find(nickName);

    if(iter != std::end(users))
    {
        User* u = &iter->second;
        return u;
    }

    return nullptr;
}

int UserBase::updateUser(User user)
{
    auto iter = users.find(user.getNickName());

    if(iter != std::end(users))
    {
        iter->second.setPublicKey(user.getPublicKey());
        return 0;
    }

    return -1;
}
