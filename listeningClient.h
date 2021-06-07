#ifndef LISTENINGCLIENT
#define LISTENINGCLIENT

#include <iostream>
#include <string>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

class listeningClient
{
    private:
        static int port;
        static std::string address;
        static int buffersize;

    public:
        listeningClient(int p, std::string a, int bs);

        void run();

};

#endif