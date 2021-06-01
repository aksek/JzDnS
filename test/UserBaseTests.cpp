//Author: Patryk Karbownik
#define BOOST_TEST_MODULE user_db_test

#include "../src/UserBase.h"
#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(addUserTest)
{
    UserBase userBase = UserBase();
    User user = User("cosik", User::UserType::NORMAL);
    BOOST_CHECK(userBase.addUser(user) == 0);
    BOOST_CHECK(userBase.addUser(user) == -1);
}

BOOST_AUTO_TEST_CASE(removeUserTest)
{
    UserBase ub = UserBase();
    BOOST_CHECK(ub.removeUser("cosik") == -1);
    User user = User("cosik", User::UserType::NORMAL);
    BOOST_CHECK(ub.removeUser("cosik") == 0);
}

BOOST_AUTO_TEST_CASE(updateUserTest)
{
    UserBase ub = UserBase();
    User us = User("cosik", User::UserType::NORMAL);
    BOOST_CHECK(ub.updateUser(us) == -1);
    ub.addUser(us);
    BOOST_CHECK(ub.updateUser(us) == 0);
}

BOOST_AUTO_TEST_CASE(getUserTest)
{
    UserBase ub = UserBase();
    BOOST_CHECK(ub.getUser("cosik") == nullptr);
    ub.addUser(User("cosik", User::UserType::NORMAL));
    BOOST_CHECK(ub.getUser("cosik") != nullptr);
}

BOOST_AUTO_TEST_CASE(serialisation_test)
{
    std::string cosik = "cosik";
    UserBase userBase = UserBase();
    User u1 = User("cosik", User::NORMAL);
    User u2 = User("cosik42", User::NORMAL);

    BOOST_CHECK(userBase.addUser(u1) == 0);
    BOOST_CHECK(userBase.addUser(u2) == 0);

    userBase.saveBaseOnDisk(cosik);
    UserBase userBase2 = UserBase();
    userBase2.loadBaseFromDisk(cosik);
    BOOST_CHECK(userBase2.getUser(u1.getNickName())->operator==(u1));
    BOOST_CHECK(userBase2.getUser(u2.getNickName())->operator==(u2));

    remove(cosik.c_str());
}