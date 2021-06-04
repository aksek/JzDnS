//Autor: Patryk Karbownik

#include "RiddleBase.h"
uint64_t  RiddleBase::addRiddle(Riddle riddle)
{
    m.lock();
    auto it = riddles.rbegin();
    uint64_t id = (riddles.empty() )? 1 : riddles.rbegin()->first + 1;
    riddle.setId(id);
    riddles.insert(std::pair<uint64_t, Riddle>(id, riddle));
    m.unlock();
    return id;
}

int RiddleBase::removeRiddle(uint64_t id)
{
    m.lock();
    auto it = riddles.find(id);

    if(it != riddles.end())
    {
        riddles.erase(it);
        m.unlock();
        return 0;
    }
    m.unlock();
    return -1;
}

Riddle RiddleBase::getRiddle(uint64_t id)
{
    Riddle r;
    m.lock();
    auto iter = riddles.find(id);

    if(iter != std::end(riddles))
    {
        r = *(&iter->second);
        m.unlock();
        return r;
    }

    m.unlock();
    throw std::runtime_error("");
}

int RiddleBase::updateRiddle(Riddle riddle)
{
    m.lock();
    auto iter = riddles.find(riddle.getId());

    if(iter != std::end(riddles))
    {
        iter->second.setAnswer(riddle.getAnswer());
        iter->second.setRiddleContent(riddle.getRiddleContent());
        m.unlock();
        return 0;
    }
    m.unlock();
    return -1;
}

void RiddleBase::loadBaseFromDisk(std::string path)
{
    m.lock();
    std::ifstream in(path);

    boost::archive::text_iarchive i_archive(in);

    i_archive >> riddles;
    m.unlock();
}

void RiddleBase::saveBaseOnDisk(std::string path)
{
    m.lock();
    std::ofstream ofs(path);

    boost::archive::text_oarchive oa(ofs);

    oa << riddles;
    m.unlock();
}

int RiddleBase::getRiddlesAmount()
{
    m.lock();
    int s = riddles.size();
    m.unlock();
    return s;
}

std::map<uint64_t, Riddle> RiddleBase::getAllRiddles()
{
    m.lock();
    std::map<uint64_t, Riddle> r = riddles;
    m.unlock();
    return r;
}

void RiddleBase::replaceRiddles(std::map<uint64_t, Riddle> riddles)
{
    m.lock();
    this->riddles = riddles;
    m.unlock();
}

RiddleBase::RiddleBase() {}
