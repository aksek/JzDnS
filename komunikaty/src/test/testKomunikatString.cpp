#include "boost/test/unit_test.hpp"
#include "message.h"

BOOST_AUTO_TEST_SUITE(TestMessageString)

BOOST_AUTO_TEST_CASE(TestMessageStringSerializeBezZmiany){
	MessageString komunikat(MessageType::Problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problem\",\"size\":0,\"control\":0},\"text\":{\"problem\":\"\"}}");

	komunikat = MessageString(MessageType::Login);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Login\",\"size\":0,\"control\":0},\"text\":{\"nick\":\"\"}}");

	komunikat = MessageString(MessageType::Solution);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Solution\",\"size\":0,\"control\":0},\"text\":{\"solution\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()
