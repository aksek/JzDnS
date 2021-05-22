#include "message.h"

#include <cstddef>
#include <stdexcept>

#include <iostream>


//Message
Message::Message(MessageType messageType): _messageType(messageType), _contentSize(0), _checksum(0){
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + contentEnum.capacity() + 1;
}

Message::Message(MessageType type, size_t size, int crc){
	_messageType = type;
	_contentSize = size;
	_checksum = crc;
}

Message::Message(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType != MessageType::Retransmit)
		throw std::runtime_error("niewłaściwy rodzaj Message");
}

std::string Message::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Retransmit){
		doc["text"] = "";
	}else throw std::runtime_error("niewłaściwy rodzaj Message");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

std::string Message::serializeEnum(){
	switch(_messageType){
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
		throw std::runtime_error("nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju");
	}
}

void Message::setContentSize(size_t contentSize){
    _contentSize = contentSize;
}

void Message::setChecksum(int checksum){
    _checksum = checksum;
}

MessageType Message::getMessageType(){
	return _messageType;
}

size_t Message::getContentSize(){
	return _contentSize;
}

int Message::getChecksum(){
	return _checksum;
}

//MessageString: public Message
MessageString::MessageString(MessageType messageType): Message(messageType), _content(""){
	if(messageType != MessageType::Problem && messageType != MessageType::Login && messageType != MessageType::Solution)
		throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + _content.capacity() + contentEnum.capacity();
}

MessageString::MessageString(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Problem){
		_content = doc["text"]["problem"].as<std::string>();
	}else if(_messageType == MessageType::Login){
		_content = doc["text"]["nick"].as<std::string>();
	}else if(_messageType == MessageType::Solution){
		_content = doc["text"]["solution"].as<std::string>();
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
}

std::string MessageString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Problem){
		doc["text"]["problem"] = _content;
	}else if(_messageType == MessageType::Login){
		doc["text"]["nick"] = _content;
	}else if(_messageType == MessageType::Solution){
		doc["text"]["solution"] = _content;
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}
	
void MessageString::setContent(std::string content){
    _content = content;
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + _content.capacity() + contentEnum.capacity();
}

std::string MessageString::getContent(){
	return _content;
}

//MessageInt: public Message
MessageInt::MessageInt(MessageType messageType): Message(messageType), _content(0){
	if(messageType != MessageType::Delete_problem)
		throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
}

MessageInt::MessageInt(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Delete_problem){
		_content = doc["text"]["id"].as<int>();
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
}

std::string MessageInt::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Delete_problem){
		doc["text"]["id"] = _content;
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageInt::setContent(int content){
    _content = content;
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
}

int MessageInt::getContent(){
	return _content;
}


//MessageBool: public Message
MessageBool::MessageBool(MessageType messageType): Message(messageType), _content(false){
	if(messageType != MessageType::Correct)
		throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
}

MessageBool::MessageBool(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Correct){
		_content = doc["text"]["correct"].as<bool>();
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
}

std::string MessageBool::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Correct){
		doc["text"]["correct"] = _content;
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageBool::setContent(bool content){
	_content = content;
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
}

bool MessageBool::getContent(){
	return _content;
}

//MessagePairStringString: public Message
MessagePairStringString::MessagePairStringString(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Round_over && messageType != MessageType::New_problem)
		throw ("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.first.capacity() + _content.second.capacity() + contentEnum.capacity();
}

MessagePairStringString::MessagePairStringString(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Round_over){
		_content.first = doc["text"]["winner_nick"].as<std::string>();
		_content.second = doc["text"]["solution"].as<std::string>();
	}else if(_messageType == MessageType::New_problem){
		_content.first = doc["text"]["problem_content"].as<std::string>();
		_content.second = doc["text"]["problem_answer"].as<std::string>();
	}else throw ("niewłaściwy rodzaj komunikatu");
}

std::string MessagePairStringString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Round_over){
		doc["text"]["winner_nick"] = _content.first;
		doc["text"]["solution"] = _content.second;
	}else if(_messageType == MessageType::New_problem){
		doc["text"]["problem_content"] = _content.first;
		doc["text"]["problem_answer"] = _content.second;
	}else throw ("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessagePairStringString::setContent(std::pair<std::string, std::string> content){
	_content = content;
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.first.capacity() + _content.second.capacity() + contentEnum.capacity();
}

std::pair<std::string, std::string> MessagePairStringString::getContent(){
	return _content;
}

//MessagePairIntString: public Message
MessagePairIntString::MessagePairIntString(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Edit_problem)
		throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.second.capacity() + contentEnum.capacity();
}

MessagePairIntString::MessagePairIntString(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Edit_problem){
		_content.first = doc["text"]["id"].as<int>();
		_content.second = doc["text"]["changed_content"].as<std::string>();
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
}

std::string MessagePairIntString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Edit_problem){
		doc["text"]["id"] = _content.first;
		doc["text"]["changed_content"] = _content.second;
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessagePairIntString::setContent(std::pair<int, std::string> content){
	_content = content;
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.second.capacity() + contentEnum.capacity();
}

std::pair<int, std::string> MessagePairIntString::getContent(){
	return _content;
}

//MessageVector: public Message
MessageVector::MessageVector(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Problems)
		throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	std::string contentEnum = serializeEnum();
	size_t rozmiarString = 0;
	for(unsigned int i=0; i<_content.size(); ++i){
		rozmiarString += _content[i].first.capacity() + _content[i].second.capacity();
	}
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(_content.size()) + _content.size()*JSON_OBJECT_SIZE(2) + rozmiarString + contentEnum.capacity();
}

MessageVector::MessageVector(MessageType type, DynamicJsonDocument &doc): Message(type, doc["header"]["size"], doc["header"]["control"]){
	if(_messageType == MessageType::Problems){
		for(JsonObject i: doc["text"].as<JsonArray>()){
			std::pair<std::string, std::string> para;
			para.first = i["text"].as<std::string>();
			para.second = i["answer"].as<std::string>();
			_content.push_back(para);
		}
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
}

std::string MessageVector::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = _contentSize;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Problems){
		if(_content.size()==0){
			JsonArray text = doc.createNestedArray("text");
		}else {
			for(unsigned int i=0; i<_content.size(); ++i){
				doc["text"][i]["text"] = _content[i].first;
				doc["text"][i]["answer"] = _content[i].second;
			}
		}
	}else throw std::runtime_error("niewłaściwy rodzaj komunikatu");
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageVector::setContent(std::vector< std::pair<std::string, std::string> > content){
	_content = content;
	std::string contentEnum = serializeEnum();
	size_t rozmiarString = 0;
	for(unsigned int i=0; i<_content.size(); ++i){
		rozmiarString += _content[i].first.capacity() + _content[i].second.capacity();
	}
	_contentSize = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(_content.size()) + _content.size()*JSON_OBJECT_SIZE(2) + rozmiarString + contentEnum.capacity();
}

std::vector< std::pair<std::string, std::string> > MessageVector::getContent(){
	return _content;
}

//Messages
Messages::Messages(){
	
}

Message* Messages::createMessage(MessageType messageType){
	if(messageType == MessageType::Problem)
		return new MessageString(messageType);
	if(messageType == MessageType::Correct)
		return new MessageBool(messageType);
	if(messageType == MessageType::Round_over)
		return new MessagePairStringString(messageType);
	if(messageType == MessageType::Retransmit)
		return new Message(messageType);
	if(messageType == MessageType::Login)
		return new MessageString(messageType);
	if(messageType == MessageType::Solution)
		return new MessageString(messageType);
	if(messageType == MessageType::Problems)
		return new MessageVector(messageType);
	if(messageType == MessageType::New_problem)
		return new MessagePairStringString(messageType);
	if(messageType == MessageType::Delete_problem)
		return new MessageInt(messageType);
	if(messageType == MessageType::Edit_problem)
		return new MessagePairIntString(messageType);
	throw std::runtime_error("nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju");
}

Message* Messages::deserialize(const std::string komunikat){
	StaticJsonDocument<64> filter;
	filter["header"]["size"] = true;

	StaticJsonDocument<96> docFilter;
	deserializeJson(docFilter, komunikat, DeserializationOption::Filter(filter));

	const size_t size = docFilter["header"]["size"];
	
	DynamicJsonDocument doc(size);
	deserializeJson(doc, komunikat);
	
	std::string type = doc["header"]["type"];
	
	Message* message;
	
	MessageType messageType = findMessageType(type);
	
	switch(messageType){
	case MessageType::Problem:
		message = new MessageString(messageType, doc);
		break;
	case MessageType::Correct:
		message = new MessageBool(messageType, doc);
		break;
	case MessageType::Round_over:
		message = new MessagePairStringString(messageType, doc);
		break;
	case MessageType::Retransmit:
		message = new Message(messageType, doc);
		break;
	case MessageType::Login:
		message = new MessageString(messageType, doc);
		break;
	case MessageType::Solution:
		message = new MessageString(messageType, doc);
		break;
	case MessageType::Problems:
		message = new MessageVector(messageType, doc);
		break;
	case MessageType::New_problem:
		message = new MessagePairStringString(messageType, doc);
		break;
	case MessageType::Delete_problem:
		message = new MessageInt(messageType, doc);
		break;
	case MessageType::Edit_problem:
		message = new MessagePairIntString(messageType, doc);
		break;
	default:
		throw std::runtime_error("nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju");
	}
	
	return message;
}

std::string Messages::serialize(Message* komunikat){
	return komunikat->serialize();
}

ValueTextMessage Messages::getText(Message* komunikat){
	switch(komunikat->getMessageType()){
	case MessageType::Problem:
		return ((MessageString*) komunikat)->getContent();
	case MessageType::Correct:
		return ((MessageBool*) komunikat)->getContent();
	case MessageType::Round_over:
		return ((MessagePairStringString*) komunikat)->getContent();
	case MessageType::Retransmit:
		return nullptr;
	case MessageType::Login:
		return ((MessageString*) komunikat)->getContent();
	case MessageType::Solution:
		return ((MessageString*) komunikat)->getContent();
	case MessageType::Problems:
		return ((MessageVector*) komunikat)->getContent();
	case MessageType::New_problem:
		return ((MessagePairStringString*)komunikat)->getContent();
	case MessageType::Delete_problem:
		return ((MessageInt*)komunikat)->getContent();
	case MessageType::Edit_problem:
		return ((MessagePairIntString*)komunikat)->getContent();
	default:
		throw std::runtime_error("nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju");
	}
}

MessageType Messages::findMessageType(std::string messageType){
	if(messageType == "Problem")
		return MessageType::Problem;
	if(messageType == "Correct")
		return MessageType::Correct;
	if(messageType == "Round_over")
		return MessageType::Round_over;
	if(messageType == "Retransmit")
		return MessageType::Retransmit;
	if(messageType == "Login")
		return MessageType::Login;
	if(messageType == "Solution")
		return MessageType::Solution;
	if(messageType == "Problems")
		return MessageType::Problems;
	if(messageType == "New_problem")
		return MessageType::New_problem;
	if(messageType == "Delete_problem")
		return MessageType::Delete_problem;
	if(messageType == "Edit_problem")
		return MessageType::Edit_problem;
	throw std::runtime_error("nie można zdeserializować komunikatu - nie ma podanego rodzaju komunikatu");
}
