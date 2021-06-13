#pragma once

#define CRYPTOPP_ENABLE_NAMESPACE_WEAK 1

#include "ArduinoJson-v6.18.0.h"
#include <cryptopp/md5.h>
#include <cryptopp/hex.h>
#include <cryptopp/rsa.h>
#include <cryptopp/secblock.h>
#include <cryptopp/osrng.h>

#include <string>
#include <vector>
#include <variant>
#include <utility>
#include <tuple>

#include <iostream>

enum class MessageType{
	Problem,
	Correct,
	Round_over,
	Retransmit,
	Login,
	Solution,
	Problems,
	New_problem,
	Delete_problem,
	Edit_problem,
	Edit_solution,
	Update,
	OK,
	Register,
	//Key,
	Login_OK,
	Login_error,
	Get_current_problem,
	Get_all_problems,
	Server_terminated
};

typedef std::variant<void*, std::string, int, bool, std::pair<std::string, std::string>, std::pair<int, std::string>, std::tuple<int, std::string, std::string>, std::vector< std::tuple<int, std::string, std::string> >, std::pair<std::string, CryptoPP::RSA::PublicKey>/*, std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock>*/ > ValueContent;

//klasa odpowiedzialna za serializację content
class SerializeContent{

public:
	SerializeContent() = default;
	std::pair<std::string, size_t> serializeString(std::string content); //size_t rozmiar wpisywany do nagłówka
	std::pair<std::string, size_t> serializeInt(int content);
	std::pair<std::string, size_t> serializeBool(bool content);
	std::pair<std::string, size_t> serializePairStringString(std::pair<std::string, std::string> content);
	std::pair<std::string, size_t> serializePairIntString(std::pair<int, std::string> content);
	std::pair<std::string, size_t> serializeTuple(std::tuple<int, std::string, std::string> content);
	std::pair<std::string, size_t> serializeVector(std::vector< std::tuple<int, std::string, std::string> > content);
	std::pair<std::string, size_t> serializePublicKey(std::pair<std::string, CryptoPP::RSA::PublicKey> content);
	//std::pair<std::string, size_t> serializeKey(std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> content);

	ValueContent deserialize(MessageType messageType, std::string contentText, size_t contentSize); //size_t rozmiar z nagłówka
	ValueContent deserialize(MessageType messageType, std::pair<std::string, size_t> content);

private:
	std::string deserializeString(std::string contentText, size_t contentSize);
	int deserializeInt(std::string contentText, size_t contentSize);
	bool deserializeBool(std::string contentText, size_t contentSize);
	std::pair<std::string, std::string> deserializePairStringString(std::string contentText, size_t contentSize);
	std::pair<int, std::string> deserializePairIntString(std::string contentText, size_t contentSize);
	std::tuple<int, std::string, std::string> deserializeTuple(std::string contentText, size_t contentSize);
	std::vector< std::tuple<int, std::string, std::string> > deserializeVector(std::string contentText, size_t contentSize);
	
public:
std::pair<std::string, CryptoPP::RSA::PublicKey> deserializePublicKey(std::string contentText, size_t contentSize);
	//std::pair<CryptoPP::SecByteBlock, CryptoPP::SecByteBlock> deserializeKey(std::string contentText, size_t contentSize);
};

//klasa odpowiedzialna za komunikaty i ich serializację
class Message{
	
public:
	Message()=default;
	Message(MessageType messageType, std::string userID);
	Message(MessageType messageType, std::string userID, std::string contentText, size_t contentSize);
	Message(MessageType messageType, std::string userID, std::pair<std::string, size_t> content);
	Message(std::string message);
	std::string serialize();
	
	MessageType getMessageType();
	std::string getUserID();
	size_t getContentSize();
	std::string getContentText();
	std::pair<std::string, size_t> getContent();
	std::string getMessageTypeString();
	
private:
	MessageType _messageType;
	std::string _userID;
	std::string _contentText;
	size_t _contentSize;
	
	static std::string messageTypeToString(MessageType messageType);
	static MessageType messageTypeFromString(std::string messageType);
};
