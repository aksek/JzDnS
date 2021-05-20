#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestKomunikatString)

BOOST_AUTO_TEST_CASE(TestKomunikatStringSerializeBezZmiany){
	KomunikatString komunikat(RodzajKomunikatu::Problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problem\",\"size\":0,\"control\":0},\"text\":{\"problem\":\"\"}}");

	komunikat = KomunikatString(RodzajKomunikatu::Login);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Login\",\"size\":0,\"control\":0},\"text\":{\"nick\":\"\"}}");

	komunikat = KomunikatString(RodzajKomunikatu::Solution);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Solution\",\"size\":0,\"control\":0},\"text\":{\"solution\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()