#include "boost/test/unit_test.hpp"
#include "message.h"

BOOST_AUTO_TEST_SUITE(TestMessageVector)

BOOST_AUTO_TEST_CASE(TestMessageVectorSerializeBezZmiany){
	MessageVector komunikat(MessageType::Problems);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":0,\"control\":0},\"text\":[]}");
}

BOOST_AUTO_TEST_SUITE_END()
