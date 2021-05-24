#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageBool)

BOOST_AUTO_TEST_CASE(TestMessageBoolSerializeBezZmiany){
	MessageBool komunikat(MessageType::Correct);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Correct\",\"size\":207,\"control\":0},\"text\":{\"correct\":false}}");
}

BOOST_AUTO_TEST_CASE(TestMessageBoolDeserializeBezZmiany){
	size_t size = 207;
	DynamicJsonDocument doc(size);
	doc["header"]["type"] = "Correct";
	doc["header"]["size"] = size;
	doc["header"]["control"] = 0;
	doc["text"]["correct"] = false;
	MessageBool komunikat(MessageType::Correct, doc);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Correct\",\"size\":207,\"control\":0},\"text\":{\"correct\":false}}");
}

BOOST_AUTO_TEST_SUITE_END()
