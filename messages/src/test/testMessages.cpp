#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessage)

BOOST_AUTO_TEST_CASE(TestMessagesSerialize){
	Messages messages;
	Message* komunikat = messages.createMessage(MessageType::Retransmit);
	BOOST_CHECK(komunikat->serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
	BOOST_CHECK(messages.serialize(komunikat)=="{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
	delete komunikat;
}

BOOST_AUTO_TEST_CASE(TestMessagesDeserialize){
	Messages messages;
	Message* komunikat = messages.deserialize("{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
	BOOST_CHECK(komunikat->serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":176,\"control\":0},\"text\":\"\"}");
	delete komunikat;
}

BOOST_AUTO_TEST_SUITE_END()