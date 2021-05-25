#include "message.h"

#include <cstddef>
#include <stdexcept>

//class SerializeContent
std::pair<std::string, size_t> SerializeContent::serializeString(std::string content){
	size_t contentSize = JSON_OBJECT_SIZE(1) + content.capacity();
	DynamicJsonDocument doc(contentSize);
	doc["value"] = content;

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 6);
}

std::pair<std::string, size_t> SerializeContent::serializeInt(int content){
	size_t contentSize = JSON_OBJECT_SIZE(1);
	DynamicJsonDocument doc(contentSize);
	doc["value"] = content;

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 6);
}

std::pair<std::string, size_t> SerializeContent::serializeBool(bool content){
	size_t contentSize = JSON_OBJECT_SIZE(1);
	DynamicJsonDocument doc(contentSize);
	doc["value"] = content;

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 6);
}

std::pair<std::string, size_t> SerializeContent::serializePairStringString(std::pair<std::string, std::string> content){
	size_t contentSize = JSON_OBJECT_SIZE(2) + content.first.capacity() + content.second.capacity();
	DynamicJsonDocument doc(contentSize);
	doc["firstValue"] = content.first;
	doc["secondValue"] = content.second;

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 23);
}

std::pair<std::string, size_t> SerializeContent::serializePairIntString(std::pair<int, std::string> content){
	size_t contentSize = JSON_OBJECT_SIZE(2) + content.second.capacity();
	DynamicJsonDocument doc(contentSize);
	doc["firstValue"] = content.first;
	doc["secondValue"] = content.second;

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 23);
}

std::pair<std::string, size_t> SerializeContent::serializeVector(std::vector< std::pair<std::string, std::string> > content){
	size_t contentSize = JSON_ARRAY_SIZE(content.size()) + content.size()*JSON_OBJECT_SIZE(2);
	for(auto i=content.begin(); i!=content.end(); ++i){
		contentSize += i->first.capacity() + i->second.capacity();
	}
	DynamicJsonDocument doc(contentSize);
	JsonArray array = doc.to<JsonArray>();
	for(int i=0; i<content.size(); ++i){
		array[i]["firstValue"] = content[i].first;
		array[i]["secondValue"] = content[i].second;
	}

	std::string serializedContent = "";
	serializeJson(doc, serializedContent);
	return std::pair<std::string, size_t>(serializedContent, contentSize + 23*content.size());
}

ValueContent SerializeContent::deserialize(MessageType messageType, std::string contentText, size_t contentSize){
	switch(messageType){
	case MessageType::Problem:
		return deserializeString(contentText, contentSize);
	case MessageType::Correct:
		return deserializeBool(contentText, contentSize);
	case MessageType::Round_over:
		return deserializePairStringString(contentText, contentSize);
	case MessageType::Retransmit:
		return nullptr;
	case MessageType::Login:
		return deserializeString(contentText, contentSize);
	case MessageType::Solution:
		return deserializeString(contentText, contentSize);
	case MessageType::Problems:
		return deserializeVector(contentText, contentSize);
	case MessageType::New_problem:
		return deserializePairStringString(contentText, contentSize);
	case MessageType::Delete_problem:
		return deserializeInt(contentText, contentSize);
	case MessageType::Edit_problem:
		return deserializePairIntString(contentText, contentSize);
	default:
		throw std::runtime_error("messageType not exist");
	}
}

ValueContent SerializeContent::deserialize(MessageType messageType, std::pair<std::string, size_t> content){
	switch(messageType){
	case MessageType::Problem:
		return deserializeString(content.first, content.second);
	case MessageType::Correct:
		return deserializeBool(content.first, content.second);
	case MessageType::Round_over:
		return deserializePairStringString(content.first, content.second);
	case MessageType::Retransmit:
		return nullptr;
	case MessageType::Login:
		return deserializeString(content.first, content.second);
	case MessageType::Solution:
		return deserializeString(content.first, content.second);
	case MessageType::Problems:
		return deserializeVector(content.first, content.second);
	case MessageType::New_problem:
		return deserializePairStringString(content.first, content.second);
	case MessageType::Delete_problem:
		return deserializeInt(content.first, content.second);
	case MessageType::Edit_problem:
		return deserializePairIntString(content.first, content.second);
	default:
		throw std::runtime_error("messageType not exist");
	}
}

std::string SerializeContent::deserializeString(std::string contentText, size_t contentSize){
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	return doc["value"].as<std::string>();
}

int SerializeContent::deserializeInt(std::string contentText, size_t contentSize){
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	return doc["value"].as<int>();
}

bool SerializeContent::deserializeBool(std::string contentText, size_t contentSize){
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	return doc["value"].as<bool>();
}

#include <iostream>

std::pair<std::string, std::string> SerializeContent::deserializePairStringString(std::string contentText, size_t contentSize){
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	std::pair<std::string, std::string> result;
	result.first = doc["firstValue"].as<std::string>();
	result.second = doc["secondValue"].as<std::string>();
	return std::pair<std::string, std::string>(doc["firstValue"].as<std::string>(), doc["secondValue"].as<std::string>());
}

std::pair<int, std::string> SerializeContent::deserializePairIntString(std::string contentText, size_t contentSize){
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	return std::pair<int, std::string>(doc["firstValue"].as<int>(), doc["secondValue"].as<std::string>());
}

std::vector< std::pair<std::string, std::string> > SerializeContent::deserializeVector(std::string contentText, size_t contentSize){
	std::vector< std::pair<std::string, std::string> > result;
	DynamicJsonDocument doc(contentSize);
	deserializeJson(doc, contentText);
	for(JsonObject i: doc.as<JsonArray>())
		result.push_back(std::pair<std::string, std::string>(i["firstValue"].as<std::string>(), i["secondValue"].as<std::string>()));
	return result;
}

//class Message
Message::Message(MessageType messageType): _messageType(messageType), _contentSize(0), _contentText(""){

}

Message::Message(MessageType messageType, std::string contentText, size_t contentSize): _messageType(messageType), _contentText(contentText), _contentSize(contentSize){
	
}

Message::Message(MessageType messageType, std::pair<std::string, size_t> content): _messageType(messageType), _contentSize(content.second), _contentText(content.first){
	
}

Message::Message(std::string message){
	const size_t size = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + message.capacity();
	DynamicJsonDocument doc(size);
	deserializeJson(doc, message);
	_messageType = messageTypeFromString(doc["header"]["type"].as<std::string>());
	_contentSize = doc["header"]["size"].as<size_t>();
	int crc = doc["header"]["control"].as<int>();
	_contentText = doc["content"].as<std::string>();
}

std::string Message::serialize(){
	std::string messageTypeString = messageTypeToString(_messageType);
	const size_t messageSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + messageTypeString.capacity() + _contentText.capacity();
	DynamicJsonDocument doc(messageSize);
	doc["header"]["type"] = messageTypeString;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = 0;
	doc["content"] = _contentText;
	
	std::string serializedMessage = "";
	serializeJson(doc, serializedMessage);
	return serializedMessage;
}
	
MessageType Message::getMessageType(){
	return _messageType;
}

size_t Message::getContentSize(){
	return _contentSize;
}

std::string Message::getContentText(){
	return _contentText;
}

std::pair<std::string, size_t> Message::getContent(){
	return std::pair<std::string, size_t>(_contentText, _contentSize);
}

std::string Message::messageTypeToString(MessageType messageType){
	switch(messageType){
	case MessageType::Problem:
		return "Problem";
	case MessageType::Correct:
		return "Correct";
	case MessageType::Round_over:
		return "Round_over";
	case MessageType::Retransmit:
		return "Retransmit";
	case MessageType::Login:
		return "Login";
	case MessageType::Solution:
		return "Solution";
	case MessageType::Problems:
		return "Problems";
	case MessageType::New_problem:
		return "New_problem";
	case MessageType::Delete_problem:
		return "Delete_problem";
	case MessageType::Edit_problem:
		return "Edit_problem";
	default:
		throw std::runtime_error("messageType not exist");
	}
}

MessageType Message::messageTypeFromString(std::string messageType){
	if(messageType == "Problem")
		return MessageType::Problem;
	else if(messageType == "Correct")
		return MessageType::Correct;
	else if(messageType == "Round_over")
		return MessageType::Round_over;
	else if(messageType == "Retransmit")
		return MessageType::Retransmit;
	else if(messageType == "Login")
		return MessageType::Login;
	else if(messageType == "Solution")
		return MessageType::Solution;
	else if(messageType == "Problems")
		return MessageType::Problems;
	else if(messageType == "New_problem")
		return MessageType::New_problem;
	else if(messageType == "Delete_problem")
		return MessageType::Delete_problem;
	else if(messageType == "Edit_problem")
		return MessageType::Edit_problem;
	else throw std::runtime_error("messageType not exist");
}