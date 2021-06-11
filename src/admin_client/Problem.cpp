#include "Problem.h"

Problem::Problem(int i, std::string q, std::string a)
{
    index = i;
    question = q;
    answer = a;
}

int Problem::getIndex()
{
    return index;
}

std::string Problem::getQuestion()
{
    return question;
}

std::string Problem::getAnswer()
{
    return answer;
}

std::string Problem::getQuestion()
{
    return question;
}

std::string Problem::getAnswer()
{
    return answer;
}

void Problem::printInfo()
{
    std::cout<< "Problem nr." << getIndex() << std::endl;
    std::cout<< getQuestion() << std::endl;
    std::cout<< "Answer: " << getAnswer() << std::endl;
}

void Problem::setQuestion(std::string q)
{
    question = q;
}

void Problem::setAnswer(std::string a)
{
    answer = a;
}
