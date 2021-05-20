#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestKomunikatVector)

BOOST_AUTO_TEST_CASE(TestKomunikatVectorSerializeBezZmiany){
	KomunikatVector komunikat(RodzajKomunikatu::Problems);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":0,\"control\":0},\"text\":[]}");
}

BOOST_AUTO_TEST_SUITE_END()