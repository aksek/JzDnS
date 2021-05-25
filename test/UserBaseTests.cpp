//Author: Patryk Karbownik
#define BOOST_TEST_MODULE user_db_test

#include "../src/UserBase.h"
#include <boost/test/included/unit_test.hpp>

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(addUserTest)
{
    UserBase userBase = UserBase();
    User user = User("cosik");
    BOOST_CHECK(userBase.addUser(user) == 0);
    BOOST_CHECK(userBase.addUser(user) == -1);
}

BOOST_AUTO_TEST_CASE(removeUserTest)
{
    UserBase ub = UserBase();
    BOOST_CHECK(ub.removeUser("cosik") == -1);
    User user = User("cosik");
    BOOST_CHECK(ub.removeUser("cosik") == 0);
}

BOOST_AUTO_TEST_CASE(updateUserTest)
{
    UserBase ub = UserBase();
    User us = User("cosik");
    BOOST_CHECK(ub.updateUser(us) == -1);
    ub.addUser(us);
    BOOST_CHECK(ub.updateUser(us) == 0);
}

BOOST_AUTO_TEST_CASE(getUserTest)
{
    UserBase ub = UserBase();
    BOOST_CHECK(ub.getUser("cosik") == nullptr);
    ub.addUser(User("cosik"));
    BOOST_CHECK(ub.getUser("cosik") != nullptr);
}
