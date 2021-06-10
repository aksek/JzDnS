//Author: Patryk Karbownik

#ifndef JZDNS_RIDDLESERVICE_H
#define JZDNS_RIDDLESERVICE_H

#include <queue>
#include "RiddleBase.h"
#include <stdexcept>
#include <map>
#include <vector>
#include <algorithm>
#include <random>

class RiddleService {
private:
    std::queue<Riddle> riddleQueue;
    Riddle lastRiddle;
    RiddleBase* riddleBase;
    void prepareRiddleQueue();
public:
    RiddleService(RiddleBase* rb);
    Riddle getNextRiddle();
    Riddle getLastRiddle();
    void restartRiddleDraw();

};


#endif //JZDNS_RIDDLESERVICE_H
