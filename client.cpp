#include "client.hpp"

int main(){
	User user;
	while(user.run()){
		;	
	}
	return 0;
}

bool User::run(){
	if(chooseServerToConnectTo()){
		int log = -1;
		while(log==-1){
			log = access();
		}
		if(log==1){
			while(showProblem(getProblem())){
				;
			}
		}
		disconnect();
		return true;	
	}else{
		disconnect();
		return false;
	}
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

void User::connection(ServerStructure serv){
	int port = serv.getPort();
	std::string address = serv.getAddress();

	int n = address.length();
	char char_addr[n+1];
	strcpy(char_addr, address.c_str());


	clientSocket = socket(AF_INET, SOCK_DGRAM, 0);
	if(clientSocket < 0){
		throw std::runtime_error("utworzenia gniazda");
	}
	
	memset(&servAddr, '\0', sizeof(servAddr));
	servAddr.sin_family = AF_INET;
	servAddr.sin_port = htons(port);
	servAddr.sin_addr.s_addr = inet_addr(char_addr);
	len = sizeof(servAddr);
}

void User::disconnect(){
	close(clientSocket);
}

Message User::sendAndRecv(std::string message){
	lastMessage = message;
	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		bufferSend[i] = '\0';
		bufferRecv[i] = '\0';
	}
	strncpy(bufferSend, message.c_str(), BUFFER_SIZE);
	sendto(clientSocket, (char*)bufferSend, BUFFER_SIZE, MSG_CONFIRM, (const struct sockaddr *) &servAddr, sizeof(servAddr));
	if(recvfrom(clientSocket, (char*)bufferRecv, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*) &servAddr, &len) < 0)
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
	sendto(clientSocket, (char*)bufferSend, BUFFER_SIZE, MSG_CONFIRM, (const struct sockaddr *) &servAddr, sizeof(servAddr));
}

Message User::recvMess(){
	for(int i = 0; i < BUFFER_SIZE; ++i)
	{
		bufferRecv[i] = '\0';
	}
	if(recvfrom(clientSocket, (char*)bufferRecv, BUFFER_SIZE, MSG_WAITALL, (struct sockaddr*) &servAddr, &len) < 0)
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
		message = createLoginMess(getLogin());
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
		message = createRegistMess(getRegist());
		if(message.size()>=BUFFER_SIZE){
			std::cout<<"Zbyt długa odpowiedz, podaj jeszcze raz odpowiedz"<<std::endl;
		}
	}while(message.size()>=BUFFER_SIZE);
	Message recvMessage = sendAndRecv(message);
	return decodeLoginAnswer(recvMessage);
}

bool User::decodeLoginAnswer(Message message){
	SerializeContent deserializer;
	if(message.getMessageType()!=MessageType::OK){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	std::string contentText = message.getContentText();
	size_t sizeContent = message.getContentSize();
	content = asymmetric_decrypt(kluczPrywatny, content, randPool);
	ValueContent value = deserializer.deserialize(MessageType::OK, contentText, sizeContent);
	int valueInt = std::get<int>(value);
	if(valueInt==1) return true;
	return false;
}

bool User::checkAnswerMessage(Message message){
	SerializeContent deserializer;
	if(message.getMessageType()!=MessageType::Correct){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	std::string contentText = message.getContentText();
	size_t sizeContent = message.getContentSize();
	content = asymmetric_decrypt(kluczPrywatny, content, randPool);
	ValueContent value = deserializer.deserialize(MessageType::Correct, contentText, sizeContent);
	return std::get<bool>(value);
}

std::string User::getProblem(){
	Message message(MessageType::Get_current_problem, nick);
	Message recvMessage = sendAndRecv(message.serialize());
	return decodeProblemMessage(recvMessage);
}

std::string User::decodeProblemMessage(Message message){
	SerializeContent deserializer;
	if(message.getMessageType()!=MessageType::Problem){
		disconnect();
		throw std::runtime_error("zly komunikat");
	}
	std::string contentText = message.getContentText();
	size_t sizeContent = message.getContentSize();
	content = asymmetric_decrypt(kluczPrywatny, content, randPool);
	ValueContent value = deserializer.deserialize(MessageType::Problem, contentText, sizeContent);
	return std::get<std::string>(value);
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

bool User::checkContinue(){
	std::cout<<"Niestety, to nie jest poprawne rozwiązanie"<<std::endl;
	do{
		std::cout<<"Chcesz spróbować jeszcze raz?"<<std::endl;
		std::cout<<"1 - tak"<<std::endl;
		std::cout<<"0 - nie"<<std::endl;
		char c;
		std::cin>>c;
		if(c=='1') return true;
		if(c=='0') return false;
		std::cout<<"Nie ma takiej opcji, wpisz '0' lub '1'"<<std::endl;
	}while(true);
}

bool User::checkNext(){
	std::cout<<"Chcesz kontynuować?"<<std::endl;
	std::cout<<"1 - tak"<<std::endl;
	std::cout<<"0 - nie"<<std::endl;
	char c;
	std::cin>>c;
	if(c=='1') return true;
	if(c=='0') return false;
	std::cout<<"Nie ma takiej opcji, wpisz '0' lub '1'"<<std::endl;
	return checkNext();
}

int User::choiceLoginOrRegist(){
	std::cout<<"Wybierz opcje:"<<std::endl;
	std::cout<<"1 - logowanie"<<std::endl;
	std::cout<<"2 - rejestracja"<<std::endl;
	std::cout<<"0 - wróć"<<std::endl;
	char c;
	std::cin>>c;
	if(c=='1') return 1;
	if(c=='2') return 2;
	if(c=='0') return 0;
	std::cout<<"Nie ma takiej opcji, wpisz '0', '1', lub '2'"<<std::endl;
	return choiceLoginOrRegist();
}

std::string createAnswerMess(std::string answer){
	SerializeContent serializer;
	std::pair<std::string, size_t> content = serializer.serializeString(answer);
	content = asymmetric_encrypt(kluczPubliczny, content, randPool);
	Message message(MessageType::Solution, nick, content);
	return message.serialize();
}

std::string User::getLogin(){
	std::cout<<"Podaj login:"<<std::endl;
	std::cin>>nick;
	return nick;
}

std::string User::createLoginMess(std::string name){
	SerializeContent serializer;
	std::pair<std::string, size_t> content = serializer.serializeString(name);
	Message message(MessageType::Login, nick, content);
	return message.serialize();
}

std::pair<std::string, CryptoPP::RSA::PublicKey> User::getRegist(){
	std::cout<<"Podaj login:"<<std::endl;
	std::cin>>nick;

	CryptoPP::RSA::PrivateKey private_key;
	CryptoPP::RSA::PublicKey public_key;
	CryptoPP::InvertibleRSAFunction params;
	params.GenerateRandomWithKeySize(randPool, 3072);
	kluczPubliczny = CryptoPP::RSA::PublicKey(params);
	kluczPrywatny = CryptoPP::RSA::PrivateKey(params);

	return std::pair<std::string, CryptoPP::RSA::PublicKey>(nick, kluczPubliczny);
}

std::string User::createRegistMess(std::pair<std::string, CryptoPP::RSA::PublicKey> dane){
	SerializeContent serializer;
	std::pair<std::string, size_t> content = serializer.serializePublicKey(dane);
	Message message(MessageType::Login, nick, content);
	return message.serialize();
}

std::vector<ServerStructure> User::getServersFromFile()
{
    std::vector<ServerStructure> servers;

    std::fstream file;
    file.open("servers.txt");
    std::string name, address, line;
    int port;
    int i = 0;

    while(std::getline(file, line))
    {
        if(i%3 == 0)
            name = line;
        else if(i%3 == 1)
            port = std::stoi(line);
        else {
            address = line;
            ServerStructure servStr = ServerStructure(name, port, address);
            servers.push_back(servStr);
        }
        i++;
    }

    file.close();

    return servers;
}

void User::showPossibleServers()
{
    std::cout << std::endl << "*********** Possible Servers: ***********" << std::endl;

    int i = 1;
    for(ServerStructure server : servers)
    {
        std::cout << "Server nr." << i  << std::endl;
        server.showInfo();
        std::cout << "-------------------------------------------" << std::endl; 
        i++;
    }
}

bool User::chooseServerToConnectTo()
{
    showPossibleServers();

    std::cout << "If you want exit, choose 0"<<std::endl;
    std::cout << "Please insert number of server you want to connect to:" << std::endl;
    std::string number;
    bool correct = false;
    int index = -1;

    do
    {
        std::getline(std::cin, number);
        if(isNumber(number))
        {
            index = stoi(number);
	    
	    if(index == 0) return false;
            if(index > servers.size() || index < 1)
                std::cout << "Incorrect server number, please try again!" << std::endl;
            else
                correct = true;
        }
        else
            std::cout << "Please enter a number!" << std::endl;
    } while (!correct);

    connection(servers[index-1]);
    return true;
}

bool User::isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

void User::roundOver(Message message){
	SerializeContent serializer;
	std::string content = message.getContentText();
	size_t sizeContent = message.getContentSize();
	content = asymmetric_decrypt(kluczPrywatny, content, randPool);
	ValueContent valueContent = serializer.deserialize(message.getMessageType(), content, sizeContent);
	std::pair<std::string, std::string> contentValue = std::get<std::pair<std::string, std::string> >(valueContent);
	std::cout<<"Niestety! zagadka już została rozwiązana przez: "<<contentValue.first<<std::endl;
	std::cout<<"Odpowiedz: "<<contentValue.second<<std::endl;
}

User::User(){
	servers = getServersFromFile();
}
