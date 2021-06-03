#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageInt)

BOOST_AUTO_TEST_CASE(TestMessageIntSerializeBezZmiany){
	MessageInt komunikat(MessageType::Delete_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Delete_problem\",\"size\":207,\"control\":0},\"text\":{\"id\":0}}");
}

BOOST_AUTO_TEST_CASE(TestMessageIntDeserializeBezZmiany){
	size_t size = 207;
	DynamicJsonDocument doc(size);
	doc["header"]["type"] = "Delete_problem";
	doc["header"]["size"] = size;
	doc["header"]["control"] = 0;
	doc["text"]["id"] = 0;
	MessageInt komunikat(MessageType::Delete_problem, doc);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Delete_problem\",\"size\":207,\"control\":0},\"text\":{\"id\":0}}");
}

BOOST_AUTO_TEST_SUITE_END()
