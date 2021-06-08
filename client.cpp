#include "client.hpp"

int main(){
	return 0;
}

Message User::sendAndRecv(std::string message){
	lastMessage = message;
	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		bufferSend[i] = '\0';
		bufferRecv[i] = '\0';
	}
	strncpy(bufferSend, message.c_str(), BUFFER_SIZE);
	send(clientSocket, bufferSend, BUFFER_SIZE, 0);
	if(recv(clientSocket, bufferRecv, BUFFER_SIZE, 0) < 0)
        	std::runtime_error("blad gniazda");

	std::string messRecv(bufferRecv);
        Message mess(messRecv);
	if(mess.getMessageType()==MessageType::Retransmit)
		return sendAndRecv(message);
	else return mess;
}

void sendMess(std::string message){
	lastMessage = message;
	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		bufferSend[i] = '\0';
	}
	strncpy(bufferSend, message.c_str(), BUFFER_SIZE);
	send(clientSocket, bufferSend, BUFFER_SIZE, 0);
}

Message User::recvMess(){
	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		bufferRecv[i] = '\0';
	}
	if(recv(clientSocket, bufferRecv, BUFFER_SIZE, 0) < 0)
        	std::runtime_error("blad gniazda");

	std::string messRecv(bufferRecv);
        Message mess(messRecv);
	if(mess.getMessageType()==MessageType::Retransmit)
		return sendAndRecv(lastMessage);
	return mess;
}

void User::login(){
	std::string message;
	do{
		message = getLogin();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	Message recvMessage = sendAndRecv(message);
	if(decodeLoginAnswer(recvMessage)){
		getProblem();
	}else{
		std::cout<<"Bad login"<<std::endl;
	}
}

void User::regist(){
	std::string message;
	do{
		message = getRegist();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	Message recvMessage = sendAndRecv(message);
	if(decodeLoginAnswer(recvMessage)){
		getProblem();
	}else{
		std::cout<<"Bad login"<<std::endl;
	}

	getProblem();
}

bool User::decodeLoginAnswer(Message message){
	if(message.getMessageType()!=MessageType::Login_OK)
		throw std::runtime_error("zly komunikat");


	return true;
}

void User::getProblem(){

	Message recvMessage = sendAndRecv("prepereMessage");
	showProblem(decodeProblemMessage(recvMessage));
}

void User::showProblem(std::string text){
	std::cout<<"Riddle:"<<std::endl;
	std::cout<<text<<std::endl;	

	pthread_t answerUser;
	pthread_create(&answerUser, NULL, &getAnswerFromUser, nullptr);

	Message message = recvMess();//zablokuje się
	
}

std::string getAnswer(){
	std::cout<<"Answer:";
	std::string answerText;
	std::cin>>answerText;
	return createAnswerMess(answerText);
}

void* getAnswerFromUser(void* arg){
	std::string message;
	do{
		message = getAnswer();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	sendMess(message);
	return nullptr;
}
