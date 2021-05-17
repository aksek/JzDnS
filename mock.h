#ifndef MOCK_H
#define MOCK_H

#include<string>
#include<vector>

class Mock
{
public:
    Mock();
    static bool isAdmin(std::string publicKeyPath);
    static std::vector<std::string> getServers();
    static bool keyIsValid(std::string publicKeyPath, std::string privateKeyPath);
    static bool checkAnswer(std::string answer);
};

#endif // MOCK_H
