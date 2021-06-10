//Autor: Patryk Karbownik

#include "AdminService.h"

AdminService::AdminService(RiddleBase *riddleBase)
{
    this->riddleBase = riddleBase;
}

std::map<int, Riddle> AdminService::getAllRiddlesFromBase()
{
    return riddleBase->getAllRiddles();
}

int AdminService::addNewRiddle(Riddle riddle)
{
    return riddleBase->addRiddle(riddle);
}

int AdminService::deleteRiddle(int riddleId)
{
    return riddleBase->removeRiddle(riddleId);
}

int AdminService::updateRiddle(Riddle riddle)
{
    return riddleBase->updateRiddle(riddle);
}

void AdminService::replaceRiddlesInBase(std::map<int, Riddle> riddles)
{
    riddleBase->replaceRiddles(riddles);
}

Riddle AdminService::getRiddle(int riddleId) {
    return riddleBase->getRiddle(riddleId);
}
