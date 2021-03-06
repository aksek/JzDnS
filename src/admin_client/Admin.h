//
// Author: Artur Mazur
//

#ifndef ADMIN_H
#define ADMIN_H

#include <iostream>
#include <vector>
#include <limits>
#include <string>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/time.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
//#include <cryptopp/rsa.h>
//#include <cryptopp/osrng.h>
#include "../cryptography.hpp"
#include "../message.h"
#include "serverStructure.h"
#include "Problem.h"
#include "fstream"
#include "BlockingQueueAdmin.hpp"
#include "../Logger.h"
//#include "message/src/message.h"

struct InfoAdmin
{
    int port;
    std::string address;
    int bufferSize;
    BlockingQueueAdmin<std::string> *queue;
};

class Admin
{
    private:
        CryptoPP::AutoSeededRandomPool rng;
        CryptoPP::RSA::PublicKey server_public_key;
        CryptoPP::RSA::PublicKey admin_public_key;
        CryptoPP::RSA::PrivateKey admin_private_key;

        std::vector<ServerStructure> servers;
        std::vector<Problem> problems;
        BlockingQueueAdmin<std::string> queue;
        bool connected;
        int maxQuestionLength;
        int maxBuffSize;
        std::string id = "Admin";
        std::thread connetion_thread;
        Logger logger;

        void * handle_connection(void * arguments);

        void showOptions();

        std::vector<ServerStructure> getServersFromFile();

        std::string insertQuestion();

        std::string insertAnswer();

        bool isNumber(const std::string& str);

        int selectProblem();

        void editProblem(int index);

        void editAnswer(int index);

        void editQuestion(int index);

        void editWholeProblem(int index);

        void connectToServer(ServerStructure serv);

    public:
        Admin(int l, int mbs);

        ~Admin();

        void run();

        void showPossibleServers();

        void chooseServerToConnectTo();

        void disconnectFromServer();

        void showAllProblems();

        void addNewProblem();

        void selectProblemToDelete();

        void selectProblemToEdit();
};

#endif
