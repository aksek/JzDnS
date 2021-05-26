//Author: Patryk Karbownik
#define BOOST_TEST_MODULE riddle_db_test
#include <boost/test/included/unit_test.hpp>
#include "../src/RiddleBase.h"
#include <cstdio>

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(addRiddle_method_check) {
    RiddleBase riddleBase = RiddleBase();
    riddleBase.addRiddle(Riddle(1, "cosik", "cosik"));
    if(riddleBase.addRiddle(Riddle(1, "cosik", "cosik")) == -1)
    {
        Riddle r = (*riddleBase.getRiddle(1));
        Riddle r2 = Riddle(1, "cosik", "cosik");
        BOOST_CHECK(r == r2);
    }
    else
    {
        BOOST_CHECK(false);
    }
}

BOOST_AUTO_TEST_CASE(remove_riddle_from_empty_db_test)
{
    RiddleBase rb = RiddleBase();
    BOOST_CHECK(rb.removeRiddle(1) == -1);
}

BOOST_AUTO_TEST_CASE(remove_riddle_test)
{
    RiddleBase rb = RiddleBase();
    rb.addRiddle(Riddle(1, "cosik", "cosik"));
    BOOST_CHECK(rb.removeRiddle(1) == 0);
    BOOST_CHECK(rb.getRiddle(1) == nullptr);
}

BOOST_AUTO_TEST_CASE(update_riddle_test)
{
    RiddleBase rb = RiddleBase();
    rb.addRiddle(Riddle(1, "cosik", "cosik"));
    Riddle riddle = Riddle(1, "cosik2", "cosik2");
    BOOST_CHECK(rb.updateRiddle(riddle) == 0);
    BOOST_CHECK(rb.getRiddle(1)->operator==(riddle));
}

BOOST_AUTO_TEST_CASE(update_not_existing_riddle_test)
{
    RiddleBase rb = RiddleBase();
    BOOST_CHECK(rb.updateRiddle(Riddle(1, "cosik", "cosik")) == -1);
}

BOOST_AUTO_TEST_CASE(get_not_existing_riddle_test)
{
    RiddleBase rb = RiddleBase();
    BOOST_CHECK(rb.getRiddle(1) == nullptr);
}

BOOST_AUTO_TEST_CASE(get_existing_riddle_test)
{
    RiddleBase rb = RiddleBase();
    Riddle riddle = Riddle(1, "cosik", "cosik");

    rb.addRiddle(riddle);
    BOOST_CHECK(rb.getRiddle(1)->operator==(riddle));
}

BOOST_AUTO_TEST_CASE(serialisation_test)
{
    std::string cosik = "cosik";
    RiddleBase rb = RiddleBase();
    Riddle r1 = Riddle(1, cosik, "cosik");
    Riddle r2 = Riddle(2, cosik, "cosik42");

    BOOST_CHECK(rb.addRiddle(r1) == 0);
    BOOST_CHECK(rb.addRiddle(r2) == 0);

    rb.saveBaseOnDisk(cosik);
    RiddleBase rb2 = RiddleBase();
    rb2.loadBaseFromDisk(cosik);
    BOOST_CHECK(rb2.getRiddle(1)->operator==(r1));
    BOOST_CHECK(rb2.getRiddle(2)->operator==(r2));

    remove(cosik.c_str());
}
