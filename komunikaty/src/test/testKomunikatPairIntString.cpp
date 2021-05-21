#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessagePairIntString)

BOOST_AUTO_TEST_CASE(TestMessagePairIntStringSerializeBezZmiany){
	MessagePairIntString komunikat(RodzajMessageu::Edit_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Edit_problem\",\"size\":0,\"control\":0},\"text\":{\"id\":0,\"changed_content\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()