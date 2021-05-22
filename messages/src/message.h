#pragma once

#include "ArduinoJson-v6.18.0.h"

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
	
	virtual std::string serialize();
	
	std::string serializeEnum();
	
	void setContentSize(int contentSize);
	void setChecksum(int checksum);
	
	int getContentSize();
	int getChecksum();
	
protected:
	MessageType _messageType;
	int _contentSize;
	int _checksum;
};

class MessageString: public Message{
	
public:
	explicit MessageString(MessageType messageType);
	std::string serialize() override;
	
	void setContent(std::string content);
	std::string getContent();

private:
	std::string _content;
};

class MessageInt: public Message{

public:
	explicit MessageInt(MessageType messageType);
	std::string serialize() override;

	void setContent(int content);
	int getContent();

private:
	int _content;
};

class MessageBool: public Message{

public:
	explicit MessageBool(MessageType messageType);
	std::string serialize() override;

	void setContent(bool content);
	bool getContent();

private:
	bool _content;
};

class MessagePairStringString: public Message{

public:
	explicit MessagePairStringString(MessageType messageType);
	std::string serialize() override;

	void setContent(std::pair<std::string, std::string> content);
	std::pair<std::string, std::string> getContent();

private:
	std::pair<std::string, std::string> _content;
};

class MessagePairIntString: public Message{

public:
	explicit MessagePairIntString(MessageType messageType);
	std::string serialize() override;

	void setContent(std::pair<int, std::string> content);
	std::pair<int, std::string> getContent();

private:
	std::pair<int, std::string> _content;
};

class MessageVector: public Message{

public:
	explicit MessageVector(MessageType messageType);
	std::string serialize() override;

	void setContent(std::vector< std::pair<std::string, std::string> > content);
	std::vector< std::pair<std::string, std::string> > getContent();

private:
	std::vector< std::pair<std::string, std::string> > _content;
};

class Messages{

public:
	Messages();
	Message* createMessage(MessageType messageType);
	Message* deserialize(std::string);
	std::string serialize(Message* komunikat);
};
