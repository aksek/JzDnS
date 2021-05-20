#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestKomunikatBool)

BOOST_AUTO_TEST_CASE(TestKomunikatBoolSerializeBezZmiany){
	KomunikatBool komunikat(RodzajKomunikatu::Correct);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Correct\",\"size\":0,\"control\":0},\"text\":{\"correct\":false}}");
}

BOOST_AUTO_TEST_SUITE_END()