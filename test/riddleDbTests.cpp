//Author: Patryk Karbownik

#define BOOST_TEST_MODULE riddle_db_test
#include <boost/test/included/unit_test.hpp>
#include "../src/server/RiddleBase.h"
#include <cstdio>

using namespace boost::unit_test;

BOOST_AUTO_TEST_CASE(addRiddle_method_check) {
    RiddleBase riddleBase;
    riddleBase.addRiddle(Riddle(1, "cosik", "cosik"));
    if(riddleBase.addRiddle(Riddle(1, "cosik", "cosik")) == -1)
    {
        Riddle r = riddleBase.getRiddle(1);
        Riddle r2 = Riddle(1, "cosik", "cosik");
        BOOST_CHECK(r == r2);
    }
    else
    {
        BOOST_CHECK(true);
    }
}

BOOST_AUTO_TEST_CASE(remove_riddle_from_empty_db_test)
{
    RiddleBase rb;
    BOOST_CHECK(rb.removeRiddle(1) == -1);
}

BOOST_AUTO_TEST_CASE(remove_riddle_test)
{
    RiddleBase rb;
    rb.addRiddle(Riddle(1, "cosik", "cosik"));
    BOOST_CHECK(rb.removeRiddle(1) == 0);
    try {
        rb.getRiddle(1);
    }
    catch (const std::runtime_error& e) {
        BOOST_CHECK(true);
    }
}

BOOST_AUTO_TEST_CASE(update_riddle_test)
{
    RiddleBase rb;
    rb.addRiddle(Riddle(1, "cosik", "cosik"));
    Riddle riddle = Riddle(1, "cosik2", "cosik2");
    BOOST_CHECK(rb.updateRiddle(riddle) == 0);
    BOOST_CHECK(rb.getRiddle(1).operator==(riddle));
}

BOOST_AUTO_TEST_CASE(update_not_existing_riddle_test)
{
    RiddleBase rb;
    BOOST_CHECK(rb.updateRiddle(Riddle(1, "cosik", "cosik")) == -1);
}

BOOST_AUTO_TEST_CASE(get_not_existing_riddle_test)
{
    RiddleBase rb;
    try {
        rb.getRiddle(1);
    }
    catch (const std::runtime_error& e) {
        BOOST_CHECK(true);
    }
}

BOOST_AUTO_TEST_CASE(get_existing_riddle_test)
{
    RiddleBase rb;
    Riddle riddle = Riddle(1, "cosik", "cosik");

    rb.addRiddle(riddle);
    BOOST_CHECK(rb.getRiddle(1).operator==(riddle));
}

BOOST_AUTO_TEST_CASE(serialisation_test2)
{
    std::string cosik = "riddleBase";
    RiddleBase rb;
    Riddle r1 = Riddle(1, "5+6", "11");
    Riddle r2 = Riddle(2, "11 - 5", "6");
    Riddle r3 = Riddle(3, "11 * 2", "22");
    Riddle r4 = Riddle(4, "5 * 11", "55");
    Riddle r5 = Riddle(5, " 5 / 5", "1");

    BOOST_CHECK(rb.addRiddle(r1) != 0);
    BOOST_CHECK(rb.addRiddle(r2) != 0);
    BOOST_CHECK(rb.addRiddle(r3) != 0);
    BOOST_CHECK(rb.addRiddle(r4) != 0);
    BOOST_CHECK(rb.addRiddle(r5) != 0);

    rb.saveBaseOnDisk(cosik);
    RiddleBase rb2;
 //   rb2.loadBaseFromDisk(cosik);
//    BOOST_CHECK(rb2.getRiddle(1).operator==(r1));
 //   BOOST_CHECK(rb2.getRiddle(2).operator==(r2));

//    remove(cosik.c_str());
}
