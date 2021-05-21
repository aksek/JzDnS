#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageString)

BOOST_AUTO_TEST_CASE(TestMessageStringSerializeBezZmiany){
	MessageString komunikat(RodzajMessageu::Problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problem\",\"size\":0,\"control\":0},\"text\":{\"problem\":\"\"}}");

	komunikat = MessageString(RodzajMessageu::Login);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Login\",\"size\":0,\"control\":0},\"text\":{\"nick\":\"\"}}");

	komunikat = MessageString(RodzajMessageu::Solution);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Solution\",\"size\":0,\"control\":0},\"text\":{\"solution\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()