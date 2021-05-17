#include "mock.h"

Mock::Mock()
{

}

bool Mock::isAdmin(std::string publicKeyPath)
{
    return publicKeyPath.find("admin") != std::string::npos;
}

std::vector<std::string> Mock::getServers()
{
    std::vector<std::string> servers = {"server1", "server2", "server3", "server4"};
    return servers;
}

bool Mock::keyIsValid(std::string publicKeyPath, std::string privateKeyPath)
{
    return true;
}

bool Mock::checkAnswer(std::string answer)
{
    return answer.compare("42") == 0;
}
