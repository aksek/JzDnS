#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessage)

BOOST_AUTO_TEST_CASE(TestMessageSerialize){
	Message komunikat(RodzajMessageu::Retransmit);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"text\":\"\"}");
}

BOOST_AUTO_TEST_SUITE_END()