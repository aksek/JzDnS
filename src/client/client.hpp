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
#include "../message.h"
#include "../cryptography.hpp"

#define BUFFER_SIZE 300

int clientSocket;
sockaddr_in servAddr;
socklen_t len;
char bufferSend[BUFFER_SIZE];
char bufferRecv[BUFFER_SIZE];
std::string lastMessage;

std::mutex userMutex;
bool wyslana;
bool odgadnieta;

std::string nick;
CryptoPP::RSA::PublicKey kluczPublicznySerwera;
CryptoPP::RSA::PublicKey kluczPubliczny;
CryptoPP::RSA::PrivateKey kluczPrywatny;
CryptoPP::AutoSeededRandomPool randPool;

//tworzenie komunikatu o rozwiązaniu
std::string createAnswerMess(std::string answer);

//wysłanie komunikatu
void sendMess(std::string message);

//odbiór rozwiązania od użytkownika
std::string getAnswer();

//wątek do odbioru rozwiązania i jego wysłania
void* getAnswerFromUser(void* arg);

class User{
private:
std::vector<ServerStructure> servers;

//pobranie od użytkownika danych do logowania
std::string getLogin();

//stworzenie komunikatu logowania
std::string createLoginMess(std::string name);

//pobranie od użytkownika danych do rejestracji
std::pair<std::string, CryptoPP::RSA::PublicKey> getRegist();

//stworzenie komunikatu danych do rejestracji
std::string createRegistMess(std::pair<std::string, CryptoPP::RSA::PublicKey> dane);

//deserializacja komunikatu po logowaniu
bool decodeLoginAnswer(Message message);

//deserializacja komunikatu z zagadką
std::string decodeProblemMessage(Message message);

//deserializacja komunikatu o poprawności rozwiązania
bool checkAnswerMessage(Message message);

//wysłanie komuikatu i odbiór odpowiedzi
Message sendAndRecv(std::string message);

//odebranie komunikatu
Message recvMess();

//obsługa komunikatu o spóźnieniu z rozwiązaniem
void roundOver(Message message);

//połaczenie z serwerem
void connection(ServerStructure);

//rozłączenie
void disconnect();

//pomocnicza - wybór sposobu weryfikacji tożsamości i jej obsługa
int access();

//pytanie o kontynuowanie po niepoprawnym rozwiązaniu
bool checkContinue();

//pytanie o kontynuowanie po zagadce
bool checkNext();

//wybranie i połączenie z serwerem
bool chooseServerToConnectTo();

//wybór logowania lub ejestracji
int choiceLoginOrRegist();

//logowanie
bool login();//wysłać i odebrać

//rejestracja
bool regist();//wysłać i odebrać

//odebranie zagadki
std::string getProblem();//wysłać i odebrać

//obsługa rozwiązywania zagadki
bool showProblem(std::string text);

//pobranie serwerów z pliku
std::vector<ServerStructure> getServersFromFile();

//wyświetlenie opcji
void showPossibleServers();

//ustalenie czy pod podanym numerem jest dany serwer
bool isNumber(const std::string& str);

public:
User();

//pętla zdarzeń
bool run();
};
