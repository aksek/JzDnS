//author: Patryk Karbownik

#include "Riddle.h"

Riddle::Riddle(uint64_t id, std::string riddleContent, std::string answer)
{
    this->id = id;
    this->riddleContent = riddleContent;
    this->answer = answer;
}

uint64_t Riddle::getId()
{
    return this->id;
}

std::string Riddle::getRiddleContent()
{
    return this->riddleContent;
}

std::string Riddle::getAnswer()
{
    return this->answer;
}

void Riddle::setId(uint64_t id)
{
    this->id = id;
}

void Riddle::setRiddleContent(std::string riddleContent)
{
    this->riddleContent = riddleContent;
}

void Riddle::setAnswer(std::string answer)
{
    this->answer = answer;
}