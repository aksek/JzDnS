#include "client.hpp"

int main(){
	User user;
	while(user.run()){
		;	
	}
	return 0;
}

bool User::run(){
	if(choiceSerwer()){
		int log = -1;
		while(log==-1){
			log = access();
		}
		if(log==1){
			while(showProblem(getProblem())){
				;
			}
		}
		return true;	
	}else return false;
}

int User::access(){
	int choice = choiceLoginOrRegist();
	if(choice==0){
		disconnect();
		return 0;
	}else if(choice==1){
		if(login())
			return 1;
		else return -1;
	}else if(choice==2){
		if(regist())
			return 1;
		else return -1;
	}else throw std::runtime_error("niezdefiniowany wybór");
}

void connection(ServerStructure serv){
	struct sockaddr_in serverAddr;

	int port = serv.getPort();
	std::string address = serv.getAddress();

	int n = address.length();
	char char_addr[n+1];
	strcpy(char_addr, address.c_str());


	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		throw std::runtime_error("utworzenia gniazda");
	}
	
	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(char_addr);

	if(connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr)) < 0){
		throw std::runtime_error("błąd połączenia");
	}
}

void User::disconnect(){

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

bool User::login(){
	std::string message;
	do{
		message = getLogin();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	Message recvMessage = sendAndRecv(message);
	return decodeLoginAnswer(recvMessage);
}

bool User::regist(){
	std::string message;
	do{
		message = getRegist();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	Message recvMessage = sendAndRecv(message);
	return decodeLoginAnswer(recvMessage);
}

bool User::decodeLoginAnswer(Message message){
	if(message.getMessageType()!=MessageType::Login_OK){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	//deserializacja message

	return true;
}

bool User::checkAnswerMessage(Message message){
	if(message.getMessageType()!=MessageType::Correct){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	//deseralizacja message

	return true;
}

std::string User::getProblem(){
	//przygotuj recvMessage
	Message recvMessage = sendAndRecv("prepereMessage");
	return decodeProblemMessage(recvMessage);
}

std::string User::decodeProblemMessage(Message message){
	if(message.getMessageType()!=MessageType::Problem){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	//deserializacja message

	return "";
}

bool User::showProblem(std::string text){
	std::cout<<"Riddle:"<<std::endl;
	std::cout<<text<<std::endl;	

	userMutex.lock();
	wyslana = false;
	odgadnieta = false;
	userMutex.unlock();

	pthread_t answerUser;
	pthread_create(&answerUser, NULL, &getAnswerFromUser, nullptr);

	Message message = recvMess();
	if(message.getMessageType()==MessageType::Round_over){
		userMutex.lock();
		odgadnieta = true;
		pthread_cancel(answerUser);
		pthread_join(answerUser, nullptr);
		roundOver(message);
		if(wyslana) recvMess();
		userMutex.unlock();
	}else if(checkAnswerMessage(message)){
		std::cout<<"Gratulacje! Jako pierwszy rozwiązałeś poprawnie zagadkę!"<<std::endl;
	}else{
		if(checkContinue()){
			 return showProblem(text);
		}else return false;
	}
	return checkNext();
}

std::string getAnswer(){
	std::cout<<"Answer:";
	std::string answerText;
	std::cin>>answerText;
	return createAnswerMess(answerText);
}

void* getAnswerFromUser(void* arg){
	pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
	std::string message;
	do{
		message = getAnswer();
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	userMutex.lock();
	if(!odgadnieta){
		sendMess(message);
		wyslana = true;
	}
	userMutex.unlock();
	return nullptr;
}
