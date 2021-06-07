#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "serverStructure.h"
#include "Problem.h"
#include "fstream"
#include "BlockingQueueAdmin.hpp"
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
        std::vector<ServerStructure> servers;
        std::vector<Problem> problems;
        BlockingQueueAdmin<std::string> queue;
        bool connected;
        int maxQuestionLength;
        int maxBuffSize;
        std::thread connetion_thread;

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

        ~Admin(){  if(connetion_thread.joinable()) connetion_thread.join(); }

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