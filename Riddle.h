//author: Patryk Karbownik
#ifndef JZDNS_RIDDLE_H
#define JZDNS_RIDDLE_H
#include "string"

class Riddle {
private:

    uint64_t id;
    std::string riddleContent;
    std::string answer;
public:

    Riddle(uint64_t id, std::string riddleContent, std::string answer);
    uint64_t getId();
    std::string getRiddleContent();
    std::string getAnswer();
    void setId(uint64_t id);
    void setRiddleContent(std::string riddleContent);
    void setAnswer(std::string answer);
};


#endif //JZDNS_RIDDLE_H
