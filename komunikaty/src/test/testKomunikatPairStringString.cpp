#include "boost/test/unit_test.hpp"
#include "komunikat.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessagePairStringString)

BOOST_AUTO_TEST_CASE(TestMessagePairStringStringSerializeBezZmiany){
	MessagePairStringString komunikat(RodzajMessageu::Round_over);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Round_over\",\"size\":0,\"control\":0},\"text\":{\"winner_nick\":\"\",\"solution\":\"\"}}");
	
	komunikat = MessagePairStringString(RodzajMessageu::New_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"New_problem\",\"size\":0,\"control\":0},\"text\":{\"problem_content\":\"\",\"problem_answer\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()