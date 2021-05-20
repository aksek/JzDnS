#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestKomunikatInt)

BOOST_AUTO_TEST_CASE(TestKomunikatIntSerializeBezZmiany){
	KomunikatInt komunikat(RodzajKomunikatu::Delete_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Delete_problem\",\"size\":0,\"control\":0},\"text\":{\"id\":0}}");
}

BOOST_AUTO_TEST_SUITE_END()