#pragma once

#include "../messages/src/ArduinoJson-v6.18.0.h"

#include <string>
#include <vector>
#include <variant>
#include <utility>

enum class MessageType{
	Problem, Correct, Round_over, Retransmit, Login, Solution, Problems, New_problem, Delete_problem, Edit_problem
};

class Message{
	
public:
	explicit Message(MessageType messageType);
	Message(MessageType type, size_t size, int crc);
	Message(MessageType type, DynamicJsonDocument &doc);
	
	virtual std::string serialize();
	
	std::string serializeEnum();
	
	void setContentSize(size_t contentSize);
	void setChecksum(int checksum);
	
	MessageType getMessageType();
	size_t getContentSize();
	int getChecksum();
	
protected:
	MessageType _messageType;
	size_t _contentSize;
	int _checksum;
};

class MessageString: public Message{
	
public:
	explicit MessageString(MessageType messageType);
	MessageString(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;
	
	void setContent(std::string content);
	std::string getContent();

private:
	std::string _content;
};

class MessageInt: public Message{

public:
	explicit MessageInt(MessageType messageType);
	MessageInt(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;

	void setContent(int content);
	int getContent();

private:
	int _content;
};

class MessageBool: public Message{

public:
	explicit MessageBool(MessageType messageType);
	MessageBool(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;

	void setContent(bool content);
	bool getContent();

private:
	bool _content;
};

class MessagePairStringString: public Message{

public:
	explicit MessagePairStringString(MessageType messageType);
	MessagePairStringString(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;

	void setContent(std::pair<std::string, std::string> content);
	std::pair<std::string, std::string> getContent();

private:
	std::pair<std::string, std::string> _content;
};

class MessagePairIntString: public Message{

public:
	explicit MessagePairIntString(MessageType messageType);
	MessagePairIntString(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;

	void setContent(std::pair<int, std::string> content);
	std::pair<int, std::string> getContent();

private:
	std::pair<int, std::string> _content;
};

class MessageVector: public Message{

public:
	explicit MessageVector(MessageType messageType);
	MessageVector(MessageType type, DynamicJsonDocument &doc);
	
	std::string serialize() override;

	void setContent(std::vector< std::pair<std::string, std::string> > content);
	std::vector< std::pair<std::string, std::string> > getContent();

private:
	std::vector< std::pair<std::string, std::string> > _content;
};

typedef std::variant<void*, std::string, int, std::pair<std::string, std::string>, std::pair<int, std::string>, std::vector< std::pair<std::string, std::string> > > ValueTextMessage;

class Messages{

public:
	Messages();
	Message* createMessage(MessageType messageType);
	Message* deserialize(const std::string komunikat);
	std::string serialize(Message* komunikat);
	ValueTextMessage getText(Message* komunikat);
	
private:
	MessageType findMessageType(std::string type);
};
