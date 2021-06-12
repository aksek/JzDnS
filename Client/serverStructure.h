#ifndef SERVERSTRUCTURE_H
#define SERVERSTRUCTURE_H

#include <iostream>

class ServerStructure
{
    private:
        std::string name;
        int port;
        std::string addres;

    public:
        ServerStructure(std::string n, int p, std::string a);

        std::string getName();

        int getPort();

        std::string getAddress();

        void showInfo();

};

#endif