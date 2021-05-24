#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessage)

BOOST_AUTO_TEST_CASE(TestMessageSerialize){
	Message komunikat(MessageType::Retransmit);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageDeserialize){
	size_t size = 176;
	DynamicJsonDocument doc(size);
	doc["header"]["type"] = "Retransmit";
	doc["header"]["size"] = size;
	doc["header"]["control"] = 0;
	doc["text"] = "";
	Message komunikat(MessageType::Retransmit, doc);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
}

BOOST_AUTO_TEST_SUITE_END()
