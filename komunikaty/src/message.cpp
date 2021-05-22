#include "message.h"

#include <cstddef>


//Message
Message::Message(MessageType messageType): _messageType(messageType), _contentSize(0), _checksum(0){
	
}

std::string Message::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + contentEnum.capacity() + 1;
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Retransmit){
		doc["text"] = "";
	}else throw "niewłaściwy rodzaj Messageu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

std::string Message::serializeEnum(){
	if(_messageType == MessageType::Problem)
		return "Problem";
	if(_messageType == MessageType::Correct)
		return "Correct";
	if(_messageType == MessageType::Round_over)
		return "Round_over";
	if(_messageType == MessageType::Retransmit)
		return "Retransmit";
	if(_messageType == MessageType::Login)
		return "Login";
	if(_messageType == MessageType::Solution)
		return "Solution";
	if(_messageType == MessageType::Problems)
		return "Problems";
	if(_messageType == MessageType::New_problem)
		return "New_problem";
	if(_messageType == MessageType::Delete_problem)
		return "Delete_problem";
	if(_messageType == MessageType::Edit_problem)
		return "Edit_problem";
	throw "nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju";
}

void Message::setContentSize(int contentSize){
    _contentSize = contentSize;
}

void Message::setChecksum(int checksum){
    _checksum = checksum;
}

int Message::getContentSize(){
	return _contentSize;
}

int Message::getChecksum(){
	return _checksum;
}

//MessageString: public Message
MessageString::MessageString(MessageType messageType): Message(messageType), _content(""){
	if(messageType != MessageType::Problem && messageType != MessageType::Login && messageType != MessageType::Solution)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessageString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + _content.capacity() + contentEnum.capacity();
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
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}
	
void MessageString::setContent(std::string content){
    _content = content;
}

std::string MessageString::getContent(){
	return _content;
}

//MessageInt: public Message
MessageInt::MessageInt(MessageType messageType): Message(messageType), _content(0){
	if(messageType != MessageType::Delete_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessageInt::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Delete_problem){
		doc["text"]["id"] = _content;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageInt::setContent(int content){
    _content = content;
}

int MessageInt::getContent(){
	return _content;
}


//MessageBool: public Message
MessageBool::MessageBool(MessageType messageType): Message(messageType), _content(false){
	if(messageType != MessageType::Correct)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessageBool::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(1) + contentEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Correct){
		doc["text"]["correct"] = _content;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageBool::setContent(bool content){
	_content = content;
}

bool MessageBool::getContent(){
	return _content;
}

//MessagePairStringString: public Message
MessagePairStringString::MessagePairStringString(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Round_over && messageType != MessageType::New_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessagePairStringString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.first.capacity() + _content.second.capacity() + contentEnum.capacity();
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
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessagePairStringString::setContent(std::pair<std::string, std::string> content){
	_content = content;
}

std::pair<std::string, std::string> MessagePairStringString::getContent(){
	return _content;
}

//MessagePairIntString: public Message
MessagePairIntString::MessagePairIntString(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Edit_problem)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessagePairIntString::serialize(){
	std::string contentEnum = serializeEnum();
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_OBJECT_SIZE(2) + _content.second.capacity() + contentEnum.capacity();
	DynamicJsonDocument doc(rozmiar);
	doc["header"]["type"] = contentEnum;
	doc["header"]["size"] = _contentSize;
	doc["header"]["control"] = _checksum;
	
	if(_messageType == MessageType::Edit_problem){
		doc["text"]["id"] = _content.first;
		doc["text"]["changed_content"] = _content.second;
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessagePairIntString::setContent(std::pair<int, std::string> content){
	_content = content;
}

std::pair<int, std::string> MessagePairIntString::getContent(){
	return _content;
}

//MessageVector: public Message
MessageVector::MessageVector(MessageType messageType): Message(messageType){
	if(messageType != MessageType::Problems)
		throw "niewłaściwy rodzaj komunikatu";
}

std::string MessageVector::serialize(){
	std::string contentEnum = serializeEnum();
	size_t rozmiarString = 0;
	for(unsigned int i=0; i<_content.size(); ++i){
		rozmiarString += _content[i].first.capacity() + _content[i].second.capacity();
	}
	const size_t rozmiar = JSON_OBJECT_SIZE(2) + JSON_OBJECT_SIZE(3) + JSON_ARRAY_SIZE(_content.size()) + _content.size()*JSON_OBJECT_SIZE(2) + rozmiarString + contentEnum.capacity();
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
	}else throw "niewłaściwy rodzaj komunikatu";
	
	std::string wynik = "";
	serializeJson(doc, wynik);
	return wynik;
}

void MessageVector::setContent(std::vector< std::pair<std::string, std::string> > content){
	_content = content;
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
	throw "nie można zserializować rodzaju komunikatu - nie ma podanego rodzaju";
}

Message* Messages::deserialize(std::string){
	return nullptr;
}

std::string Messages::serialize(Message* komunikat){
	return komunikat->serialize();
}
