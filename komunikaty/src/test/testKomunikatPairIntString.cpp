#include "boost/test/unit_test.hpp"
#include "message.h"

BOOST_AUTO_TEST_SUITE(TestMessagePairIntString)

BOOST_AUTO_TEST_CASE(TestMessagePairIntStringSerializeBezZmiany){
	MessagePairIntString komunikat(MessageType::Edit_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Edit_problem\",\"size\":0,\"control\":0},\"text\":{\"id\":0,\"changed_content\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()
