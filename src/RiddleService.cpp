//Author: Patryk Karbownik

#include "Looper.hpp"
#include "RiddleService.h"

RiddleService::RiddleService(RiddleBase* rb)
{
    riddleBase = rb;
    prepareRiddleQueue();
}

Riddle RiddleService::getNextRiddle()
{
    if(riddleQueue.empty())
    {
        prepareRiddleQueue();
    }

    Riddle r = riddleQueue.front();
    lastRiddle = r;
    riddleQueue.pop();
    return r;
}

void RiddleService::restartRiddleDraw()
{
    prepareRiddleQueue();
}

void RiddleService::prepareRiddleQueue()
{
    if(riddleBase->getRiddlesAmount() == 0)
        throw std::domain_error("emptyBase");

    auto riddles_m = riddleBase->getAllRiddles();
    std::vector<Riddle> v_riddles;

    for(auto it = riddles_m.begin(); it != riddles_m.end(); ++it)
        v_riddles.push_back(it->second);

    std::shuffle(std::begin(v_riddles), std::end(v_riddles), std::default_random_engine());

    for(auto it = v_riddles.begin(); it != v_riddles.end(); ++it)
        riddleQueue.push(*it);

    lastRiddle = getNextRiddle();
}

Riddle RiddleService::getLastRiddle()
{
    return lastRiddle;
}