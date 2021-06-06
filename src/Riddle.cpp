//author: Patryk Karbownik

#include "Riddle.h"

Riddle::Riddle(int id, std::string riddleContent, std::string answer)
{
    this->id = id;
    this->riddleContent = riddleContent;
    this->answer = answer;
}

int Riddle::getId()
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

void Riddle::setId(int id)
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

Riddle::Riddle()
{
    id = 0;
    riddleContent = "";
    answer = "";
}

bool Riddle::operator==(const Riddle &rhs) const {
    return id == rhs.id &&
           riddleContent == rhs.riddleContent &&
           answer == rhs.answer;
}

bool Riddle::operator!=(const Riddle &rhs) const {
    return !(rhs == *this);
}

Riddle::Riddle(const Riddle &r) {
    this->id = r.id;
    this->riddleContent = r.riddleContent;
    this->answer = r.answer;
}
