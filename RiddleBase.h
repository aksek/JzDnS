//author: Patryk Karbownik

#ifndef JZDNS_RIDDLEBASE_H
#define JZDNS_RIDDLEBASE_H

#include "Riddle.h"
#include <map>

class RiddleBase {
private:
    std::map<uint64_t, Riddle> riddles;
public:
    int addRiddle(Riddle riddle);
    int removeRiddle(uint64_t id);
    Riddle* getRiddle(uint64_t id);
    int updateRiddle(Riddle riddle);
    int loadBaseFromDisk(std::string path);
    int saveBaseOnDisk(std::string path);
};


#endif //JZDNS_RIDDLEBASE_H
