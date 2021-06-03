#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessageVector)

BOOST_AUTO_TEST_CASE(TestMessageVectorSerializeBezZmiany){
	MessageVector komunikat(MessageType::Problems);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":175,\"control\":0},\"text\":[]}");
}

BOOST_AUTO_TEST_CASE(TestMessageVectorDeserializeBezZmiany){
	size_t size = 175;
	DynamicJsonDocument doc(size);
	doc["header"]["type"] = "Problems";
	doc["header"]["size"] = size;
	doc["header"]["control"] = 0;
	JsonArray text = doc.createNestedArray("text");
	MessageVector komunikat(MessageType::Problems, doc);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":175,\"control\":0},\"text\":[]}");
}

BOOST_AUTO_TEST_SUITE_END()
