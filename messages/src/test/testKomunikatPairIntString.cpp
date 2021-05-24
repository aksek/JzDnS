#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessagePairIntString)

BOOST_AUTO_TEST_CASE(TestMessagePairIntStringSerializeBezZmiany){
	MessagePairIntString komunikat(MessageType::Edit_problem);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Edit_problem\",\"size\":254,\"control\":0},\"text\":{\"id\":0,\"changed_content\":\"\"}}");
}

BOOST_AUTO_TEST_CASE(TestMessagePairIntStringDeserializeBezZmiany){
	size_t size = 254;
	DynamicJsonDocument doc(size);
	doc["header"]["type"] = "Edit_problem";
	doc["header"]["size"] = size;
	doc["header"]["control"] = 0;
	doc["text"]["id"] = 0;
	doc["text"]["changed_content"] = "";
	MessagePairIntString komunikat(MessageType::Edit_problem, doc);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Edit_problem\",\"size\":254,\"control\":0},\"text\":{\"id\":0,\"changed_content\":\"\"}}");
}

BOOST_AUTO_TEST_SUITE_END()
