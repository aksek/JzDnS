//Autor: Patryk Karbownik

#include "AdminService.h"

AdminService::AdminService(RiddleBase *riddleBase)
{
    this->riddleBase = riddleBase;
}

std::map<uint64_t, Riddle> AdminService::getAllRiddlesFromBase()
{
    return riddleBase->getAllRiddles();
}

int AdminService::addNewRiddle(Riddle riddle)
{
    return riddleBase->addRiddle(riddle);
}

int AdminService::deleteRiddle(uint64_t riddleId)
{
    return riddleBase->removeRiddle(riddleId);
}

int AdminService::updateRiddle(Riddle riddle)
{
    return riddleBase->updateRiddle(riddle);
}

void AdminService::replaceRiddlesInBase(std::map<uint64_t, Riddle> riddles)
{
    riddleBase->replaceRiddles(riddles);
}

Riddle AdminService::getRiddle(uint64_t riddleId) {
    return riddleBase->getRiddle(riddleId);
}
