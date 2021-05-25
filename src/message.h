#pragma once

#include "ArduinoJson-v6.18.0.h"

#include <string>
#include <vector>
#include <variant>
#include <utility>

enum class MessageType{
	Problem, Correct, Round_over, Retransmit, Login, Solution, Problems, New_problem, Delete_problem, Edit_problem
};

typedef std::variant<void*, std::string, int, bool, std::pair<std::string, std::string>, std::pair<int, std::string>, std::vector< std::pair<std::string, std::string> > > ValueContent;

//klasa odpowiedzialna za serializację content
class SerializeContent{

public:
	SerializeContent() = default;
	std::pair<std::string, size_t> serializeString(std::string content); //size_t rozmiar wpisywany do nagłówka
	std::pair<std::string, size_t> serializeInt(int content);
	std::pair<std::string, size_t> serializeBool(bool content);
	std::pair<std::string, size_t> serializePairStringString(std::pair<std::string, std::string> content);
	std::pair<std::string, size_t> serializePairIntString(std::pair<int, std::string> content);
	std::pair<std::string, size_t> serializeVector(std::vector< std::pair<std::string, std::string> > content);
	ValueContent deserialize(MessageType messageType, std::string contentText, size_t contentSize); //size_t rozmiar z nagłówka
	ValueContent deserialize(MessageType messageType, std::pair<std::string, size_t> content);

private:
	std::string deserializeString(std::string contentText, size_t contentSize);
	int deserializeInt(std::string contentText, size_t contentSize);
	bool deserializeBool(std::string contentText, size_t contentSize);
	std::pair<std::string, std::string> deserializePairStringString(std::string contentText, size_t contentSize);
	std::pair<int, std::string> deserializePairIntString(std::string contentText, size_t contentSize);
	std::vector< std::pair<std::string, std::string> > deserializeVector(std::string contentText, size_t contentSize);
};

//klasa odpowiedzialna za komunikaty i ich serializację
class Message{
	
public:
	Message(MessageType messageType);
	Message(MessageType messageType, std::string contentText, size_t contentSize);
	Message(MessageType messageType, std::pair<std::string, size_t> content);
	Message(std::string message);
	std::string serialize();
	
	MessageType getMessageType();
	size_t getContentSize();
	std::string getContentText();
	std::pair<std::string, size_t> getContent();
	
private:
	MessageType _messageType;
	std::string _contentText;
	size_t _contentSize;
	
	static std::string messageTypeToString(MessageType messageType);
	static MessageType messageTypeFromString(std::string messageType);
};