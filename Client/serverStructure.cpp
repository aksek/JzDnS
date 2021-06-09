#include "serverStructure.h"

ServerStructure::ServerStructure(std::string n, int p, std::string a)
{
    name = n;
    port = p;
    addres = a;
}

std::string ServerStructure::getName() { return name; }

int ServerStructure::getPort() { return port; }

std::string ServerStructure::getAddress() { return addres; }

void ServerStructure::showInfo()
{
    std::cout << "Server: " << getName() << std::endl;
    std::cout << "Port: " << getPort() << std::endl;
    std::cout << "Address: " << getAddress() << std::endl;
}