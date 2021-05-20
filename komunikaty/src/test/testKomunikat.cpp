#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestKomunikat)

BOOST_AUTO_TEST_CASE(TestKomunikatSerialize){
	Komunikat komunikat(RodzajKomunikatu::Retransmit);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"text\":\"\"}");
}

BOOST_AUTO_TEST_SUITE_END()