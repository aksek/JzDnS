#ifndef ADMIN_H
#define ADMIN_H

#include <vector>
#include <limits>
#include <string>
#include <iostream>
#include "serverStructure.h"
#include "Problem.h"
#include "fstream"
//#include "message/src/message.h"

class Admin
{
    private:
        std::vector<ServerStructure> servers;
        std::vector<Problem> problems;
        bool connected;
        int maxQuestionLength;

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
        Admin(int l);

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