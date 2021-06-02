//Autor: Patryk Karbownik

#ifndef JZDNS_ADMINSERVICE_H
#define JZDNS_ADMINSERVICE_H


#include "RiddleBase.h"

class AdminService {
private:
    RiddleBase* riddleBase;
public:
    AdminService(RiddleBase* riddleBase);
    std::map<uint64_t, Riddle> getAllRiddlesFromBase();
    void replaceRiddlesInBase(std::map<uint64_t, Riddle> riddles);
    int addNewRiddle(Riddle riddle);
    int updateRiddle(Riddle riddle);
    int deleteRiddle(uint64_t riddleId);
    Riddle getRiddle(uint64_t riddleId);
};


#endif //JZDNS_ADMINSERVICE_H
