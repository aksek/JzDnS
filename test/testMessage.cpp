#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestMessage)

BOOST_AUTO_TEST_CASE(TestMessageGetOnlyType){
	Message komunikat(MessageType::Retransmit, "idUser");
	BOOST_CHECK(komunikat.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat.getUserID()=="idUser");
	BOOST_CHECK(komunikat.getContentSize()==0);
	BOOST_CHECK(komunikat.getContentText()=="");
	BOOST_CHECK(komunikat.getContent().first=="");
	BOOST_CHECK(komunikat.getContent().second==0);
}

BOOST_AUTO_TEST_CASE(TestMessageGet){
	Message komunikat1(MessageType::Retransmit, "idUser", "", 0);
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getUserID()=="idUser");
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2(MessageType::Problems, "idUser", "{\"klucz\":\"wartosc\"}", 56);
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getUserID()=="idUser");
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_CASE(TestMessageGetPair){
	Message komunikat1(MessageType::Retransmit, "idUser",  std::pair<std::string, size_t>("", 0));
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getUserID()=="idUser");
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2(MessageType::Problems, "idUser",  std::pair<std::string, size_t>("{\"klucz\":\"wartosc\"}", 56));
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getUserID()=="idUser");
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_CASE(TestMessageSerializeOnlyType){
	Message komunikat(MessageType::Retransmit, "idUser");
	BOOST_CHECK(komunikat.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"user\":\"idUser\",\"size\":0,\"control\":\"D41D8CD98F00B204E9800998ECF8427E\"},\"content\":\"\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageSerialize){
	Message komunikat1(MessageType::Retransmit, "idUser", "", 0);
	BOOST_CHECK(komunikat1.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"user\":\"idUser\",\"size\":0,\"control\":\"D41D8CD98F00B204E9800998ECF8427E\"},\"content\":\"\"}");
	Message komunikat2(MessageType::Problems, "idUser", "{\"klucz\":\"wartosc\"}", 56);
	BOOST_CHECK(komunikat2.serialize()=="{\"header\":{\"type\":\"Problems\",\"user\":\"idUser\",\"size\":56,\"control\":\"B8D57F65B5A31B41B49F5D6170CA32FC\"},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageSerializePair){
	Message komunikat1(MessageType::Retransmit, "idUser",  std::pair<std::string, size_t>("", 0));
	BOOST_CHECK(komunikat1.serialize()=="{\"header\":{\"type\":\"Retransmit\",\"user\":\"idUser\",\"size\":0,\"control\":\"D41D8CD98F00B204E9800998ECF8427E\"},\"content\":\"\"}");
	Message komunikat2(MessageType::Problems, "idUser",  std::pair<std::string, size_t>("{\"klucz\":\"wartosc\"}", 56));
	BOOST_CHECK(komunikat2.serialize()=="{\"header\":{\"type\":\"Problems\",\"user\":\"idUser\",\"size\":56,\"control\":\"B8D57F65B5A31B41B49F5D6170CA32FC\"},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
}

BOOST_AUTO_TEST_CASE(TestMessageDeserialize){
	Message komunikat1("{\"header\":{\"type\":\"Retransmit\",\"user\":\"idUser\",\"size\":0,\"control\":\"D41D8CD98F00B204E9800998ECF8427E\"},\"content\":\"\"}");
	BOOST_CHECK(komunikat1.getMessageType()==MessageType::Retransmit);
	BOOST_CHECK(komunikat1.getUserID()=="idUser");
	BOOST_CHECK(komunikat1.getContentSize()==0);
	BOOST_CHECK(komunikat1.getContentText()=="");
	BOOST_CHECK(komunikat1.getContent().first=="");
	BOOST_CHECK(komunikat1.getContent().second==0);
	Message komunikat2("{\"header\":{\"type\":\"Problems\",\"user\":\"idUser\",\"size\":56,\"control\":\"B8D57F65B5A31B41B49F5D6170CA32FC\"},\"content\":\"{\\\"klucz\\\":\\\"wartosc\\\"}\"}");
	BOOST_CHECK(komunikat2.getMessageType()==MessageType::Problems);
	BOOST_CHECK(komunikat2.getUserID()=="idUser");
	BOOST_CHECK(komunikat2.getContentSize()==56);
	BOOST_CHECK(komunikat2.getContentText()=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().first=="{\"klucz\":\"wartosc\"}");
	BOOST_CHECK(komunikat2.getContent().second==56);
}

BOOST_AUTO_TEST_SUITE_END()
