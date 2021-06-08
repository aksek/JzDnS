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
#include <mutex>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include "serverStructure.h"
#include "fstream"
#include "message.h"

#define BUFFER_SIZE 300

int clientSocket;
char bufferSend[BUFFER_SIZE];
char bufferRecv[BUFFER_SIZE];

std::string lastMessage;
std::mutex userMutex;
bool wyslana;
bool odgadnieta;

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
std::string decodeProblemMessage(Message message);
bool checkAnswerMessage(Message message);
Message sendAndRecv(std::string message);
Message recvMess();
void roundOver(Message message){};
void connection(ServerStructure);
void disconnect();
int access();

public:
User(){};
bool choiceSerwer(){return true;};
int choiceLoginOrRegist(){return 0;};
bool login();//wysłać i odebrać
bool regist();//wysłać i odebrać
std::string getProblem();//wysłać i odebrać
bool showProblem(std::string text);
bool run();
bool checkContinue(){return true;};
bool checkNext(){return true;};
};
