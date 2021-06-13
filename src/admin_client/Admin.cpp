#include "Admin.h"
#include "../message.h"
#include "../cryptography.hpp"

void * Admin::handle_connection(void * arguments)
{
    int port = ((struct InfoAdmin *)arguments)->port;
    std::string address = ((struct InfoAdmin *)arguments)->address;
    int buffersize = ((struct InfoAdmin *)arguments)->bufferSize;
    BlockingQueueAdmin<std::string>* queue = ((struct InfoAdmin *)arguments)->queue;

    int sockfd;
    char buffer[buffersize];
//    char *hello = "Hello from client";
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

    while(true)
    {
        queue->lockServer();

        std::string result;

        queue->waitAndPop(result);

        strcpy(buffer, result.c_str());
        
        sendto(sockfd, (char *)buffer, strlen(buffer),
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

        struct timeval tv;
        tv.tv_sec = 10;
        tv.tv_usec = 0;
        fd_set readfds;
        FD_ZERO(&readfds);
        FD_SET(sockfd, &readfds);

        int ret = select(sockfd + 1, &readfds, NULL, NULL, &tv);

        if(ret > 0)
        {
            n = recvfrom(sockfd, (char *)buffer, buffersize, 
                    MSG_WAITALL, (struct sockaddr *) &servaddr,
                    &len);

            if(n<0)
            {
                printf("[-]Error in receiving data.\n");
                std::string erro = "0"; 
                queue->push(erro);
                queue->unlockAdmin();
            }
            else
            {
                std::string response(buffer);
                queue->push(response);
                queue->unlockAdmin();
            }
        }  
        else if(ret == 0)
        {
            printf("Timeout.\n");
            std::string erro = "0"; 
            queue->push(erro);
            queue->unlockAdmin();
        }
        
    }
}

Admin::Admin(int l, int mbs) : logger("AdminUser" + to_string(std::time(nullptr)))
{
    servers = getServersFromFile();
    Cryptography::load_public_key(admin_public_key, "admin_public_key.pem");
    Cryptography::load_private_key(admin_private_key, "admin_private_key.pem");
    connected = false;
    maxQuestionLength = l;
    maxBuffSize = mbs;
    logger.write("Opening admin module");
}

Admin::~Admin()
{
    logger.write("Closing admin module");

    if(connected)
        disconnectFromServer();

    if(connetion_thread.joinable()) 
        connetion_thread.join();

    logger.close();
}

std::vector<ServerStructure> Admin::getServersFromFile()
{
    Cryptography::load_public_key(server_public_key, "public_key.pem");

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
    std::cout << std::endl << "*********** Available Servers: ***********" << std::endl;

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

    SerializeContent serializer;
    auto serialized_login_message_content = serializer.serializePublicKey(std::make_pair(id, admin_public_key));
    Message login(MessageType::Register, id, serialized_login_message_content);
    std::string serialized_message = login.serialize();

    logger.write("Trying to connect to server : " + serv.getName() + " on port: " + std::to_string(serv.getPort()) + " with address: " + serv.getAddress() );

    int i = 0;

    for(; i < 5; i++)
    {
        queue.push(serialized_message);

        queue.unlockServer();

        connetion_thread = std::thread(&Admin::handle_connection, this, &ia);
        std::string result;

        queue.lockAdmin();
        queue.waitAndPop(result);

        if(result == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");   
        }
        else
        {
            Message mess1(result);

            if(mess1.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmition");
            }
            else if(mess1.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess1.getContentText();
                size_t sizeContent = mess1.getContentSize();

                auto num = std::get<int>(serializer.deserialize(MessageType::OK, contentText, sizeContent));

                if(num == 2)
                {
                    logger.write("Connected");
                    std::cout << "Connected" << std::endl;
                    break;
                }
            }
        }
    }

    if(i == 5)
    {
        std::cout << "Error while trying to login to server!" << std::endl;
        logger.write("Error while trying to login to server!");
        disconnectFromServer();
        return;
    }

//    std::string newContentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText1, rng);

    Message m(MessageType::Get_all_problems, id);
    std::string toSend = m.serialize();
//    Cryptography::asymmetric_encrypt(server_public_key, toSend, rng);

    logger.write("Sending request for all problems from server");

    for(int k = 0; k < 5; k++)
    {
        queue.push(toSend);
        queue.unlockServer();

        queue.lockAdmin();
        std::string content;
        queue.waitAndPop(content);

        if(content == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");
        }
        else
        {
            Message mess(content);

            if(mess.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmition");
            }
            else if(mess.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess.getContentText();
                size_t sizeContent = mess.getContentSize();

            //    contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                logger.write("Received problems from server");

                auto serverProblems = std::get<std::vector<std::tuple<int, std::string, std::string> >>(serializer.deserialize(MessageType::Problems, contentText, sizeContent));
                int num;
                std::string question, answer;
                for(int j = 0; j < serverProblems.size(); ++j)
                {
                    num = std::get<0>(serverProblems[j]);
                    question = std::get<1>(serverProblems[j]);
                    answer = std::get<2>(serverProblems[j]);
                    Problem p(num, question, answer);
                    problems.push_back(p);
                }

                std::cout << "Received problems from server" << std::endl;

                return;
            }
        }
    }

    logger.write("Sending request for problems failed!");
    disconnectFromServer();
}

void Admin::disconnectFromServer()
{
    if(!connected)
    {
        std::cout << "You are not connected to any server!" << std::endl;
        return;
    }

    std::string s = ":exit";

    logger.write("Disconnecting from server");

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

        logger.write("Sending new problem");

        for(int i = 0; i < 5; ++i)
        {
            SerializeContent sc;
            std::pair<std::string, size_t> serializedContent = sc.serializePairStringString(p);
            Message m(MessageType::New_problem, id, serializedContent);
            std::string toSend = m.serialize();
            queue.push(toSend);
            queue.unlockServer();
            queue.lockAdmin();
            std::string receive;
            queue.waitAndPop(receive);

            if(receive == "0")
            {
                logger.write("Timeout");
                logger.write("Retransmition");
            }
            else
            {
                Message mess(receive);

                if(mess.getMessageType() == MessageType::Retransmit)
                {
                    logger.write("Retransmiting");
                }
                else if(mess.getMessageType() == MessageType::Server_terminated)
                {
                    logger.write("Server was terminated");
                    disconnectFromServer();
                    std::cout << "Server was terminated" << std::endl;
                    return;
                }
                else
                {

                    std::string contentText = mess.getContentText();
                    size_t sizeContent = mess.getContentSize();

            //        contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                    auto num = std::get<int>(sc.deserialize(MessageType::OK, contentText, sizeContent));

                    Problem pr(num, q, a);

                    logger.write("New problem added correctly!");

                    problems.push_back(pr);

                    return;
                }
            }
        }

        std::cout << "Error in receiving problems! Disconnecting from server" << std::endl;
        logger.write("Sending request for problems failed!");
        disconnectFromServer();
    }
    else
        std::cout << "You should connect to a server first!" << std::endl;
}

std::string Admin::insertQuestion()
{
    std::cout << "Please write question:" << std::endl;
    std::string quest;
    std::getline(std::cin, quest);
    while(!( quest.length() > 0 && quest.length() <  maxQuestionLength))
    {
        std::cout << "Question length should be between 1 and " << maxQuestionLength << "!" << std::endl;
        std::cout << "Please try again" << std::endl;
        quest = "";
        std::getline(std::cin, quest);
    }
    return quest;
}

std::string Admin::insertAnswer()
{
    std::cout << "Please insert an answer to question" << std::endl;
    std::string k = "";
    std::getline(std::cin, k);
    while(!isNumber(k))
    {
        std::cout << "Please insert a number! Try again!" << std::endl;
        k = "";
        std::getline(std::cin, k);
    }
    return k;
}

bool Admin::isNumber(const std::string& str)
{
    for(char const &c : str) {
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

    if(problems.empty())
    {
        std::cout << "Problem list is empty!" << std::endl;
        return;
    }

    showAllProblems();
    std::cout << std::endl << "Insert a number of problems to delete:" << std::endl;

    int n = selectProblem();


    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeInt(n);
    Message m(MessageType::New_problem, id, serializedContent);
    std::string toSend = m.serialize();

    for(int i = 0; i < 5; ++i)
    {
        queue.push(toSend);
        queue.unlockServer();
        queue.lockAdmin();
        std::string content;
        queue.waitAndPop(content);

        if(content == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");
        }
        else
        {
            
            Message mess(content);

            if(mess.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmiting");
            }
            else if(mess.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess.getContentText();
                size_t sizeContent = mess.getContentSize();

            //    contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                auto num = std::get<int>(sc.deserialize(MessageType::OK, contentText, sizeContent));

                if(num == -1)
                {
                    logger.write("Error during delete");
                    logger.write("Retransmiting");
                }
                else
                {
                    logger.write("Successful delete");
                    logger.write("Deleted problem nr" + std::to_string(n));
                    std::cout << "Deleted problem nr" << n << std::endl;
                    problems.erase(problems.begin() + n);
                    return;
                }
            }
        }
    }

    std::cout << "Error in delete! Disconnecting from server" << std::endl;
    disconnectFromServer();
    logger.write("Cannot successfully end deleting");
}

void Admin::selectProblemToEdit()
{
    std::cout << std::endl << "*********** Edit problem: ***********" << std::endl;

    if(!connected)
    {
        std::cout << "You should be connected first!" << std::endl;
        return;
    }

    if(problems.empty())
    {
        std::cout << "Problem list is empty!" << std::endl;
        return;
    }

    showAllProblems();
    std::cout << std::endl << "Input the index of the problem to edit:" << std::endl;

    int n = selectProblem();

    editProblem(n);

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

    logger.write("Updating question of problem nr" + std::to_string(problems[index].getIndex()));
	
    std::tuple<int, std::string, std::string> t(problems[index].getIndex(), newQuestion, problems[index].getAnswer());
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeTuple(t);
    Message m(MessageType::Update, id, serializedContent);
    std::string toSend = m.serialize();

    for(int i = 0; i < 5; i++)
    {

        queue.push(toSend);
        queue.unlockServer();
        queue.lockAdmin();
        std::string receive;
        queue.waitAndPop(receive);

        if(receive == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");
        }
        else
        {
            Message mess(receive);

            if(mess.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmiting");
            }
            else if(mess.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess.getContentText();
                size_t sizeContent = mess.getContentSize();

            //    contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                auto num = std::get<int>(sc.deserialize(MessageType::OK, contentText, sizeContent));

                if(num == -1)
                {
                    logger.write("Error during update");
                    logger.write("Retransmiting");
                }
                else
                {
                    logger.write("Successful update");
                    return;
                }
            }
        }
    }

    std::cout << "Error in updating! Disconnecting from server" << std::endl;
    disconnectFromServer();
    logger.write("Cannot successfully end updating");
}

void Admin::editAnswer(int index)
{
    problems[index].printInfo();
    std::string newAnswer = insertAnswer();
    problems[index].setAnswer(newAnswer);

    logger.write("Updating answer of problem nr" + std::to_string(problems[index].getIndex()));
    
    std::tuple<int, std::string, std::string> t(problems[index].getIndex(), problems[index].getQuestion(), newAnswer);
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeTuple(t);
    Message m(MessageType::Update, id, serializedContent);
    std::string toSend = m.serialize();

    for(int i = 0; i < 5; ++i)
    {

        queue.push(toSend);
        queue.unlockServer();
        queue.lockAdmin();
        std::string receive;
        queue.waitAndPop(receive);

        if(receive == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");
        }
        else
        {
            Message mess(receive);

            if(mess.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmiting!");
            }
            else if(mess.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess.getContentText();
                size_t sizeContent = mess.getContentSize();

            //    contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                SerializeContent serializer;
                auto num = std::get<int>(serializer.deserialize(MessageType::OK, receive, receive.size()));

                if(num == -1)
                {
                    logger.write("Error during update");
                    logger.write("Retransmiting");
                }
                else
                {
                    logger.write("Successful update");
                    return;
                }
            }
        }
    }

    std::cout << "Error in updating! Disconnecting from server" << std::endl;
    logger.write("Cannot successfully end updating");
    disconnectFromServer();

}

void Admin::editWholeProblem(int index)
{
    problems[index].printInfo();
    std::string newQuestion = insertQuestion();
    problems[index].setQuestion(newQuestion);
    std::string newAnswer = insertAnswer();
    problems[index].setAnswer(newAnswer);

    logger.write("Updating whole problem nr" + std::to_string(problems[index].getIndex()));
    
    std::tuple<int, std::string, std::string> t(problems[index].getIndex(), newQuestion, newAnswer);
    SerializeContent sc;
    std::pair<std::string, size_t> serializedContent = sc.serializeTuple(t);
    Message m(MessageType::Edit_solution, id, serializedContent);
    std::string toSend = m.serialize();

    for(int i = 0; i < 5; ++i)
    {
        queue.push(toSend);
        queue.unlockServer();
        queue.lockAdmin();
        std::string receive;
        queue.waitAndPop(receive);

        if(receive == "0")
        {
            logger.write("Timeout");
            logger.write("Retransmition");
        }
        else
        {
            Message mess(receive);

            if(mess.getMessageType() == MessageType::Retransmit)
            {
                logger.write("Retransmiting!");
            }
            else if(mess.getMessageType() == MessageType::Server_terminated)
            {
                logger.write("Server was terminated");
                disconnectFromServer();
                std::cout << "Server was terminated" << std::endl;
                return;
            }
            else
            {
                std::string contentText = mess.getContentText();
                size_t sizeContent = mess.getContentSize();

            //    contentText = Cryptography::asymmetric_decrypt(admin_private_key, contentText, rng);

                SerializeContent serializer;

                auto num = std::get<int>(serializer.deserialize(MessageType::OK, receive, receive.size()));

                if(num == -1)
                {
                    logger.write("Error during update");
                    logger.write("Retransmiting");
                }
                else
                {
                    logger.write("Successful update");
                    return;
                }
            }
        }
    }

    std::cout << "Error in updating! Disconnecting from server" << std::endl;
    logger.write("Cannot successfully end updating");
    disconnectFromServer();

}

void Admin::showAllProblems()
{
    std::cout << std::endl << "*********** All problems: ***********" << std::endl << std::endl;

    if(problems.empty())
    {
        std::cout << "There are no problems to show!" << std::endl;
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
    std::cout << "--------Welcome to Administrator module--------" << std::endl;
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
                std::cout<< "Unknown option, please try again!" << std::endl;
                
        }
    }
}
