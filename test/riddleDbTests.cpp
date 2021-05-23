//Author: Patryk Karbownik
#define BOOST_TEST_MODULE riddle_db_test
#include <boost/test/included/unit_test.hpp>
#include "../src/RiddleBase.h"

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

//Lista testow:
//proba usuniecia nie istniejacej zagadki
//proba usuniecia istniejacej zagadki
//proba aktualizacji istniejacej zagadki
//proba aktualizacji nieisteniejacej zagadki
//proba pobrania nieistniejacej zagadki
//proba pobrania istniejacej zagadki
//serializacja