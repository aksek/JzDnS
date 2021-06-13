//Autor: Patryk Karbownik

#ifndef JZDNS_ADMINSERVICE_H
#define JZDNS_ADMINSERVICE_H


#include "RiddleBase.h"

class AdminService {
private:
    RiddleBase* riddleBase;
public:
    AdminService(RiddleBase* riddleBase);
    std::map<int, Riddle> getAllRiddlesFromBase();
    void replaceRiddlesInBase(std::map<int, Riddle> riddles);
    int addNewRiddle(Riddle riddle);
    int updateRiddle(Riddle riddle);
    int deleteRiddle(int riddleId);
    Riddle getRiddle(int riddleId);
};


#endif //JZDNS_ADMINSERVICE_H
