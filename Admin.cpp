#include "Admin.h"
#include "src/message.h"

void * Admin::handle_connection(void * arguments)
{
    int port = ((struct InfoAdmin *)arguments)->port;
    std::string address = ((struct InfoAdmin *)arguments)->address;
    int buffersize = ((struct InfoAdmin *)arguments)->bufferSize;
    BlockingQueueAdmin<std::string>* queue = ((struct InfoAdmin *)arguments)->queue;
/*
    int clientSocket, ret;
	struct sockaddr_in serverAddr;
	char buffer[buffersize];

    int n = address.length();
    char char_addr[n+1];
    strcpy(char_addr, address.c_str());

	for(int i = 0; i < buffersize; ++i)
	{
		buffer[i] = '\0';
	}

	clientSocket = socket(AF_INET, SOCK_STREAM, 0);
	if(clientSocket < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Client Socket is created.\n");

	memset(&serverAddr, '\0', sizeof(serverAddr));
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(port);
	serverAddr.sin_addr.s_addr = inet_addr(char_addr);

	ret = connect(clientSocket, (struct sockaddr*)&serverAddr, sizeof(serverAddr));
	if(ret < 0){
		printf("[-]Error in connection.\n");
		exit(1);
	}
	printf("[+]Connected to Server.\n");


	while(1)
    {
        queue->lockServer();

        std::string result;

        queue->waitAndPop(result);

        strcpy(buffer, result.c_str());
        send(clientSocket, buffer, strlen(buffer), 0);

        if(strcmp(buffer, ":exit") == 0){
            close(clientSocket);
            printf("[-]Disconnected from server.\n");
            queue->unlockAdmin();
            queue->unlockServer();
            break;
        }

        for(int i = 0; i < buffersize; ++i)
	    {
		    buffer[i] = '\0';
	    }

        if(recv(clientSocket, buffer, buffersize, 0) < 0){
            printf("[-]Error in receiving data.\n");
        }else{
            std::string response(buffer);
            queue->push(response);
            queue->unlockAdmin();
        }
        
	}
 */

    int sockfd;
    char buffer[buffersize];
    char *hello = "Hello from client";
    struct sockaddr_in     servaddr;
  
    if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
        perror("socket creation failed");
        exit(EXIT_FAILURE);
    }
  
    memset(&servaddr, 0, sizeof(servaddr));

    int k = address.length();
    char char_addr[k+1];
    strcpy(char_addr, address.c_str());

    for(int i = 0; i < buffersize; ++i)
	{
		buffer[i] = '\0';
	}

    servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(port);
	servaddr.sin_addr.s_addr = inet_addr(char_addr);
      
    int n;
    socklen_t len = sizeof(servaddr); 

    while(1)
    {
        queue->lockServer();

        std::string result;

        queue->waitAndPop(result);

        strcpy(buffer, result.c_str());
        
        sendto(sockfd, (char *)buffer, strlen(hello),
            MSG_CONFIRM, (const struct sockaddr *) &servaddr, 
                sizeof(servaddr));

        if(strcmp(buffer, ":exit") == 0){
            close(sockfd);
            printf("[-]Disconnected from server.\n");
            queue->unlockAdmin();
            queue->unlockServer();
            break;
        }

        for(int i = 0; i < buffersize; ++i)
        {
            buffer[i] = '\0';
        }
            
        n = recvfrom(sockfd, (char *)buffer, buffersize, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);

        if(n<0)
            printf("[-]Error in receiving data.\n");
        else{
            std::string response(buffer);
            queue->push(response);
            queue->unlockAdmin();
        }
    }
}

Admin::Admin(int l, int mbs)
{
    servers = getServersFromFile();
    connected = false;
    maxQuestionLength = l;
    maxBuffSize = mbs;
}

std::vector<ServerStructure> Admin::getServersFromFile()
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

void Admin::showPossibleServers()
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

void Admin::chooseServerToConnectTo()
{
    std::cout << std::endl << "*********** Connect to server: ***********" << std::endl;

    if(connected)
    {
        std::cout << "You are already connected! Cannot connect to more servers!" << std::endl;
        return;
    }

    showPossibleServers();

    std::cout << "Please insert number of server you want to connect to:" << std::endl;
    std::cout << "or [q] to quit" << std::endl;
    std::string number;
    bool correct = false;
    int index = -1;

    do
    {
        std::getline(std::cin, number);
        if(isNumber(number))
        {
            index = stoi(number);

            if(index > servers.size() || index < 1)
                std::cout << "Incorrect server number, please try again!" << std::endl;
            else
                correct = true;
        }
        else if(number[0] == 'q')
            return;
        else
            std::cout << "Please enter a number!" << std::endl;
    } while (!correct);

    connectToServer(servers[index-1]);
}

void Admin::connectToServer(ServerStructure serv)
{
    std::cout << "Trying to connect to: " << std::endl;
    serv.showInfo();

    struct InfoAdmin ia = {serv.getPort(), serv.getAddress(), maxBuffSize, &queue };
    connected = true;

    queue.lockServer();
    queue.lockAdmin();

    std::string s = "hello";

    queue.push(s);

    queue.unlockServer();

    connetion_thread = std::thread(&Admin::handle_connection, this, &ia);
    std::string result;

    queue.lockAdmin();
    queue.waitAndPop(result);


    SerializeContent serializer;
   
    Message m(MessageType::Get_all_problems, "Admin");
    std::string toSend = m.serialize();

    

    std::string receive = sendToServer(toSend);

    Message response(receive);

    auto serverProblems = std::get<std::vector<std::tuple<int, std::string, std::string> > >(serializer.deserialize(MessageType::Problems, response.getContent()));
    int num;
    std::string question, answer;
    for(int i = 0; i < serverProblems.size(); ++i)
    {
        num = std::get<0>(serverProblems[i]);
        question = std::get<1>(serverProblems[i]);
        answer = std::get<2>(serverProblems[i]);
        Problem p(num, question, answer);
        problems.push_back(p);
    }
   

}

void Admin::disconnectFromServer()
{
    if(!connected)
    {
        std::cout << "You are not connected to any server!" << std::endl;
        return;
    }

    std::string s = ":exit";
    queue.push(s);

    queue.unlockServer();

    connected = false;

    if(connetion_thread.joinable()) connetion_thread.join();

}

void Admin::addNewProblem()
{
    if(connected)
    {
        std::cout << std::endl << "*********** Add new problem: ***********" << std::endl;

        std::string q = insertQuestion(); 
        std::string a = insertAnswer();

        int number = 3;
        std::pair<std::string, std::string> p(q,a);
        /*
        SerializeContent sc;
        std::pair<std::string, size_t> serializedContent = sc.serializePairStringString(p);
        Message m(MessageType::New_problem, 101, serializedContent);
        std::string toSend = m.serialize();
         queue.push(toSend);
         queue.unlockServer();
         queue.lockAdmin();
        std::string receive;
         queue.waitAndPop(receive);
        !!!!!!!!!!!!! deserialize !!!!!!!!!!!!!!!!!!!
        int number = deserializedMess.deserialize(content, size); !!!!!!!
        */

        Problem pr(number, q, a);

        problems.push_back(pr);
    }
    else
        std::cout << "You should connect to server first!" << std::endl;
}

std::string Admin::insertQuestion()
{
    std::cout << "Please write question:" << std::endl;
    std::string quest;
    std::getline(std::cin, quest);
    while(!( quest.length() > 0 && quest.length() <  maxQuestionLength))
    {
        std::cout << "Length of question should be between 1 and " << maxQuestionLength << "!" << std::endl;
        std::cout << "Pleas try again" << std::endl;
        quest = "";
        std::getline(std::cin, quest);
    }
    return quest;
}

std::string Admin::insertAnswer()
{
    std::cout << "Please insert a answer to question" << std::endl;
    std::string k = "";
    std::getline(std::cin, k);
    while(!isNumber(k))
    {
        std::cout << "Pleas inser a number! Try again!" << std::endl;
        k = "";
        std::getline(std::cin, k);
    }
    return k;
}

bool Admin::isNumber(const std::string& str)
{
    for (char const &c : str) {
        if (std::isdigit(c) == 0) return false;
    }
    return true;
}

int Admin::selectProblem()
{
    std::string number;
    int n;
    bool correct = false;
    int index = -1;

    do
    {
        std::getline(std::cin, number);
        if(isNumber(number))
        {
            n = stoi(number);
            for(int i = 0; i < problems.size(); ++i)
            {
                if(problems[i].getIndex() == n)
                    index = i;
            }

            if(index == -1)
                std::cout << "There is no problem with number " << n << "! Try again!" << std::endl;
            else
                correct = true;
        }
        else
            std::cout << "Please enter a number!" << std::endl;
    } while (!correct);

    return index;
}

void Admin::selectProblemToDelete()
{
    std::cout << std::endl << "*********** Delete problem: ***********" << std::endl;


    if(!connected)
    {
        std::cout << "You should be connected first!" << std::endl;
        return;
    }

    if(problems.size() < 1)
    {
        std::cout << "Problem list is empty!" << std::endl;
        return;
    }

    showAllProblems();
    std::cout << std::endl << "Insert a number of problem to delete:" << std::endl;

    int n = selectProblem();

    /*
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeInt(int n);
    Message m(MessageType::New_problem, 101, serializedContent);
    std::string toSend = m.serialize();
    queue.push(toSend);
    queue.unlockServer();
    queue.lockAdmin();
    std::string receive;
    queue.waitAndPop(receive);
    !!!!!!!!!!!!! deserialize !!!!!!!!!!!!!!!!!!!
    int number = deserializedMess.deserialize(content, size); !!!!!!!
    */

    problems.erase(problems.begin() + n);
}

void Admin::selectProblemToEdit()
{
    std::cout << std::endl << "*********** Edit problem: ***********" << std::endl;

    if(!connected)
    {
        std::cout << "You should be connected first!" << std::endl;
        return;
    }

    if(problems.size() < 1)
    {
        std::cout << "Problem list is empty!" << std::endl;
        return;
    }

    showAllProblems();
    std::cout << std::endl << "Insert a number of problem to delete:" << std::endl;

    int n = selectProblem();

    editProblem(n);

    return;
}

void Admin::editProblem(int index)
{
    std::cout << std::endl << "Type: " << std::endl;
    std::cout << "[0]\tto edit question\n";
    std::cout << "[1]\tto edit answer\n";
    std::cout << "[2]\tto edit whole problem\n";
    std::cout << "[q]\tto quit\n";
    char c;
    std::cin >> c;
    while(std::cin.get()>31) {};
    switch(c)
    {
        case '0':
            editQuestion(index);
            break;
        case '1':
            editAnswer(index);
            break;
        case '2':
            editWholeProblem(index);
            break;
        default:
            std::cout << "Unknown option! Type 0, 1 or 2!" << std::endl;
    }
}

void Admin::editQuestion(int index)
{
    problems[index].printInfo();
    std::string newQuestion = insertQuestion();
    problems[index].setQuestion(newQuestion);
    std::pair<int, std::string> p(problems[index].getIndex(), newQuestion);

    /*
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializePairIntString(p);
    Message m(MessageType::Edit_problem, 101, serializedContent);
    std::string toSend = m.serialize();
    queue.push(toSend);
    queue.unlockServer();
    queue.lockAdmin();
    std::string receive;
    queue.waitAndPop(receive);
    !!!!!!!!!!!!! deserialize !!!!!!!!!!!!!!!!!!!
    int number = deserializedMess.deserialize(content, size); !!!!!!!
    */
}

void Admin::editAnswer(int index)
{
    problems[index].printInfo();
    std::string newAnswer = insertAnswer();
    problems[index].setAnswer(newAnswer);
    std::pair<int, std::string> p(problems[index].getIndex(), newAnswer);

    /*
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializePairIntString(p);
    Message m(MessageType::Edit_solution, 101, serializedContent);
    std::string toSend = m.serialize();
    queue.push(toSend);
    queue.unlockServer();
    queue.lockAdmin();
    std::string receive;
    queue.waitAndPop(receive);
    !!!!!!!!!!!!! deserialize !!!!!!!!!!!!!!!!!!!
    int number = deserializedMess.deserialize(content, size); !!!!!!!
    */
}

void Admin::editWholeProblem(int index)
{
    problems[index].printInfo();
    std::string newQuestion = insertQuestion();
    problems[index].setQuestion(newQuestion);
    std::string newAnswer = insertAnswer();
    problems[index].setAnswer(newAnswer);
    
    /*
    std::tuple<int, std::string, std::string> t(problems[index].getIndex(), newQuestion, newAnswer);
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeTuple(t);
    Message m(MessageType::Edit_solution, 101, serializedContent);
    std::string toSend = m.serialize();
    queue.push(toSend);
    queue.unlockServer();
    queue.lockAdmin();
    std::string receive;
    queue.waitAndPop(receive);
    !!!!!!!!!!!!! deserialize !!!!!!!!!!!!!!!!!!!
    int number = deserializedMess.deserialize(content, size); !!!!!!!
    */
}

void Admin::showAllProblems()
{
    std::cout << std::endl << "*********** All problems: ***********" << std::endl << std::endl;

    if( problems.size() < 1)
    {
        std::cout << "There is no problem to show!" << std::endl;
        return;
    }

    for(Problem p: problems)
    {
        p.printInfo();
        std::cout << "-------------------------------------------" << std::endl; 
    }
}

void Admin::showOptions()
{
    std::cout << std::endl;
    std::cout << "Press: " << std::endl;
    std::cout << "[0]\tto see possible servers\n";
    std::cout << "[1]\tto connect to server\n";
    std::cout << "[2]\tto disconnect form server\n";
    std::cout << "[3]\tto see all problems on server\n";
    std::cout << "[4]\tto add new problem\n";
    std::cout << "[5]\tto select and delete problem\n";
    std::cout << "[6]\tto select and edit problem\n";
    std::cout << "[7]\tto see help\n";
    std::cout << "[8]\tto quit\n";
    std::cout << std::endl;
}

void Admin::run()
{
    std::cout << "--------Welcome in Administrator module--------" << std::endl;
    showOptions();
    bool continueWork = true;

    while(continueWork)
    {
        std::cout << std::endl << "Choose option (type 7 for help)" << std::endl;
        char c;
        std::cin >> c;
        while(std::cin.get()>31) {};

        switch(c)
        {
            case '0':
                showPossibleServers();
                break;
            case '1':
                chooseServerToConnectTo();
                break;
            case '2':
                disconnectFromServer();
                break;
            case '3':
                showAllProblems();
                break;
            case '4':
                addNewProblem();
                break;
            case '5':
                selectProblemToDelete();
                break;
            case '6':
                selectProblemToEdit();
                break;
            case '7':
                showOptions();
                break;
            case '8':
                continueWork = false;
                break;
            default:
                std::cout<< "Unknown option, please give correct one!" << std::endl;
                
        }
    }
}
