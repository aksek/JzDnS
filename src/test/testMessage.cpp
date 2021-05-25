#include "boost/test/unit_test.hpp"
#include "message.h"

BOOST_AUTO_TEST_SUITE(TestMessage)

BOOST_AUTO_TEST_CASE(TestMessageGetOnlyType){
	Message komunikat(MessageType::Retransmit);
	BOOST_CHECK(komunikat.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat.getContentSize()==0);
	BOOST_CHECK(komunikat.getContentText()=="");
	BOOST_CHECK(komunikat.getContent().first=="");
	BOOST_CHECK(komunikat.getContent().second==0);
}

BOOST_AUTO_TEST_CASE(TestMessageGet){
	Message komunikat1(MessageType::Retransmit, "", 0);
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2(MessageType::Problems, "{\"klucz\":\"wartosc\"}", 56);
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_CASE(TestMessageGetPair){
	Message komunikat1(MessageType::Retransmit, std::pair<std::string, size_t>("", 0));
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2(MessageType::Problems, std::pair<std::string, size_t>("{\"klucz\":\"wartosc\"}", 56));
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_CASE(TestMessageSerializeOnlyType){
	Message komunikat(MessageType::Retransmit);
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"content\":\"\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageSerialize){
	Message komunikat1(MessageType::Retransmit, "", 0);
	BOOST_CHECK(komunikat1.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"content\":\"\"}");
	Message komunikat2(MessageType::Problems, "{\"klucz\":\"wartosc\"}", 56);
	BOOST_CHECK(komunikat2.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":56,\"control\":0},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageSerializePair){
	Message komunikat1(MessageType::Retransmit, std::pair<std::string, size_t>("", 0));
	BOOST_CHECK(komunikat1.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"content\":\"\"}");
	Message komunikat2(MessageType::Problems, std::pair<std::string, size_t>("{\"klucz\":\"wartosc\"}", 56));
	BOOST_CHECK(komunikat2.serialize()=="{\"header\":{\"type\":\"Problems\",\"size\":56,\"control\":0},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageDeserialize){
	Message komunikat1("{\"header\":{\"type\":\"Retransmit\",\"size\":0,\"control\":0},\"content\":\"\"}");
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2("{\"header\":{\"type\":\"Problems\",\"size\":56,\"control\":0},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_SUITE_END()
