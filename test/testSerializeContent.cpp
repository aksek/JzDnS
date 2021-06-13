#include "boost/test/unit_test.hpp"
#include "message.h"

#include <iostream>

BOOST_AUTO_TEST_SUITE(TestSerializeContent)

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializeString){
	SerializeContent serialize;
	std::string napis = "napis";
	BOOST_CHECK(serialize.serializeString(napis).first=="{\"value\":\"napis\"}");
	BOOST_CHECK(serialize.serializeString(napis).second==53);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializeInt){
	SerializeContent serialize;
	int wartosc = 73;
	BOOST_CHECK(serialize.serializeInt(wartosc).first=="{\"value\":73}");
	BOOST_CHECK(serialize.serializeInt(wartosc).second==38);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializeBool){
	SerializeContent serialize;
	bool wartosc = true;
	BOOST_CHECK(serialize.serializeBool(wartosc).first=="{\"value\":true}");
	BOOST_CHECK(serialize.serializeBool(wartosc).second==38);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializePairStringString){
	SerializeContent serialize;
	std::pair<std::string, std::string> wartosc("napis1", "napis2");
	BOOST_CHECK(serialize.serializePairStringString(wartosc).first=="{\"firstValue\":\"napis1\",\"secondValue\":\"napis2\"}");
	BOOST_CHECK(serialize.serializePairStringString(wartosc).second==117);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializePairIntString){
	SerializeContent serialize;
	std::pair<int, std::string> wartosc(52, "napis2");
	BOOST_CHECK(serialize.serializePairIntString(wartosc).first=="{\"firstValue\":52,\"secondValue\":\"napis2\"}");
	BOOST_CHECK(serialize.serializePairIntString(wartosc).second==102);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializeVector){
	SerializeContent serialize;
	std::vector<std::tuple<int, std::string, std::string> > wartosc;
	BOOST_CHECK(serialize.serializeVector(wartosc).first=="[]");
	BOOST_CHECK(serialize.serializeVector(wartosc).second==0);
	wartosc.push_back(std::tuple<int, std::string, std::string>(1, "napis1", "napis2"));
	BOOST_CHECK(serialize.serializeVector(wartosc).first=="[{\"firstValue\":1,\"secondValue\":\"napis1\",\"thirdValue\":\"napis2\"}]");
	BOOST_CHECK(serialize.serializeVector(wartosc).second==192);
	wartosc.push_back(std::tuple<int, std::string, std::string>(2, "napis3", "napis4"));
	BOOST_CHECK(serialize.serializeVector(wartosc).first=="[{\"firstValue\":1,\"secondValue\":\"napis1\",\"thirdValue\":\"napis2\"},{\"firstValue\":2,\"secondValue\":\"napis3\",\"thirdValue\":\"napis4\"}]");
	BOOST_CHECK(serialize.serializeVector(wartosc).second==384);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentSerializePublicKey){
CryptoPP::AutoSeededRandomPool rng;
CryptoPP::RSA::PrivateKey private_key;
CryptoPP::RSA::PublicKey public_key;
CryptoPP::InvertibleRSAFunction params;
params.GenerateRandomWithKeySize(rng, 3072);
public_key = CryptoPP::RSA::PublicKey(params);
private_key = CryptoPP::RSA::PrivateKey(params);

std::string login = "login_nazwa";
SerializeContent serialize;

std::pair<std::string, CryptoPP::RSA::PublicKey> para(login, public_key);
std::pair<std::string, size_t> zserializowany(serialize.serializePublicKey(para));
std::string napis1 = zserializowany.first;

para = serialize.deserializePublicKey(zserializowany.first, zserializowany.second);
zserializowany = serialize.serializePublicKey(para);
std::string napis2 = zserializowany.first;

BOOST_CHECK(napis1==napis2);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializeNull){
	SerializeContent serialize;
	std::string message = "";
	size_t size = 0;
	BOOST_CHECK(std::get<void*>(serialize.deserialize(MessageType::Retransmit, message, size))==nullptr);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializeString){
	SerializeContent serialize;
	std::string message = "{\"value\":\"napis\"}";
	size_t size = 53;
	BOOST_CHECK(std::get<std::string>(serialize.deserialize(MessageType::Problem, message, size))=="napis");
	BOOST_CHECK(std::get<std::string>(serialize.deserialize(MessageType::Login, message, size))=="napis");
	BOOST_CHECK(std::get<std::string>(serialize.deserialize(MessageType::Solution, message, size))=="napis");
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializeInt){
	SerializeContent serialize;
	std::string message = "{\"value\":23}";
	size_t size = 38;
	BOOST_CHECK(std::get<int>(serialize.deserialize(MessageType::Delete_problem, message, size))==23);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializeBool){
	SerializeContent serialize;
	std::string message = "{\"value\":true}";
	size_t size = 38;
	BOOST_CHECK(std::get<bool>(serialize.deserialize(MessageType::Correct, message, size))==true);
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializePairStringString){
	SerializeContent serialize;
	std::string message = "{\"firstValue\":\"napis1\",\"secondValue\":\"napis2\"}";
	size_t size = 117;
	BOOST_CHECK((std::get<std::pair<std::string, std::string> >(serialize.deserialize(MessageType::Round_over, message, size))).first=="napis1");
	BOOST_CHECK((std::get<std::pair<std::string, std::string> >(serialize.deserialize(MessageType::Round_over, message, size))).second=="napis2");
	BOOST_CHECK((std::get<std::pair<std::string, std::string> >(serialize.deserialize(MessageType::New_problem, message, size))).first=="napis1");
	BOOST_CHECK((std::get<std::pair<std::string, std::string> >(serialize.deserialize(MessageType::New_problem, message, size))).second=="napis2");
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializePairIntString){
	SerializeContent serialize;
	std::string message = "{\"firstValue\":145,\"secondValue\":\"napis2\"}";
	size_t size = 102;
	BOOST_CHECK((std::get<std::pair<int, std::string> >(serialize.deserialize(MessageType::Edit_problem, message, size))).first==145);
	BOOST_CHECK((std::get<std::pair<int, std::string> >(serialize.deserialize(MessageType::Edit_problem, message, size))).second=="napis2");
}

BOOST_AUTO_TEST_CASE(TestSerializeContentDeserializeVector){
	SerializeContent serialize;
	std::string message = "[]";
	size_t size = 0;
	std::vector<std::tuple<int, std::string, std::string> > wektor = std::get<std::vector<std::tuple<int, std::string, std::string> > >(serialize.deserialize(MessageType::Problems, message, size));
	BOOST_CHECK(wektor.size()==0);
	message = "[{\"firstValue\":1,\"secondValue\":\"napis1\",\"thirdValue\":\"napis2\"}]";
	size = 192;
	wektor = std::get<std::vector<std::tuple<int, std::string, std::string> > >(serialize.deserialize(MessageType::Problems, message, size));
	BOOST_CHECK(wektor.size()==1);
	BOOST_CHECK(std::get<0>(wektor[0])==1);
	BOOST_CHECK(std::get<1>(wektor[0])=="napis1");
	BOOST_CHECK(std::get<2>(wektor[0])=="napis2");
	message = "[{\"firstValue\":1,\"secondValue\":\"napis1\",\"thirdValue\":\"napis2\"},{\"firstValue\":2,\"secondValue\":\"napis3\",\"thirdValue\":\"napis4\"}]";
	size = 384;
	wektor = std::get<std::vector<std::tuple<int, std::string, std::string> > >(serialize.deserialize(MessageType::Problems, message, size));
	BOOST_CHECK(wektor.size()==2);
	BOOST_CHECK(std::get<0>(wektor[0])==1);
	BOOST_CHECK(std::get<1>(wektor[0])=="napis1");
	BOOST_CHECK(std::get<2>(wektor[0])=="napis2");
	BOOST_CHECK(std::get<0>(wektor[1])==2);
	BOOST_CHECK(std::get<1>(wektor[1])=="napis3");
	BOOST_CHECK(std::get<2>(wektor[1])=="napis4");
}

BOOST_AUTO_TEST_SUITE_END()
