//
// Created by patryk on 19.05.2021.
//

#include "RiddleBase.h"
int RiddleBase::addRiddle(Riddle riddle)
{
    auto it = riddles.find(riddle.getId());
    if(it == riddles.end())
    {
        riddles.insert(std::pair<uint64_t, Riddle>(riddle.getId(), riddle));
        return 0;
    }
    return -1;
}

int RiddleBase::removeRiddle(uint64_t id)
{
    auto it = riddles.find(id);

    if(it != riddles.end())
    {
        riddles.erase(it);
        return 0;
    }
    return -1;
}

Riddle* RiddleBase::getRiddle(uint64_t id)
{
    auto iter = riddles.find(id);

    if(iter != std::end(riddles))
    {
        Riddle* r = &iter->second;
        return r;
    }

    return nullptr;
}

int RiddleBase::updateRiddle(Riddle riddle)
{
    auto iter = riddles.find(riddle.getId());

    if(iter != std::end(riddles))
    {
        iter->second.setAnswer(riddle.getAnswer());
        iter->second.setRiddleContent(riddle.getRiddleContent());

        return 0;
    }

    return -1;
}

int RiddleBase::loadBaseFromDisk(std::string path)
{

}

int RiddleBase::saveBaseOnDisk(std::string path)
{

}