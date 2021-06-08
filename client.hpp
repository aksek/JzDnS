#pragma once

#include <iostream>
#include <cstring>
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
#include <sys/msg.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "serverStructure.h"
#include "fstream"
#include "message.h"

#define BUFFER_SIZE 300

int clientSocket, ret;
char bufferSend[BUFFER_SIZE];
char bufferRecv[BUFFER_SIZE];

struct Problem{
std::string text;
std::string answer;
};

std::string lastMessage;

std::string createAnswerMess(std::string answer){return answer;}
void sendMess(std::string message);
void* getAnswerFromUser(void* arg);
std::string getAnswer();


class User{
private:
std::vector<ServerStructure> servers;

std::string getLogin(){return "";};
std::string createLoginMess(std::string name){return "";};
std::string getRegist(){return "";};
std::string createRegistMess(std::string name){return "";};
bool decodeLoginAnswer(Message message);
std::string decodeProblemMessage(Message message){return "";};
Message sendAndRecv(std::string message);
Message recvMess();

public:
User(){};
void choiceSerwer(){};
void connection(ServerStructure){};
void disconnect(){};
void login();//wysłać i odebrać
void regist();//wysłać i odebrać
void getProblem();//wysłać i odebrać
void showProblem(std::string text);
};
