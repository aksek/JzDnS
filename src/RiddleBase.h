//author: Patryk Karbownik

#ifndef JZDNS_RIDDLEBASE_H
#define JZDNS_RIDDLEBASE_H

#include "Riddle.h"
#include "boost/serialization/map.hpp"
#include <fstream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/utility.hpp>
#include <boost/archive/xml_oarchive.hpp>
#include <boost/archive/xml_iarchive.hpp>
#include <mutex>

class RiddleBase {
private:
    std::map<int, Riddle> riddles;
    std::mutex m;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(riddles);
    }
public:
    RiddleBase();

    int addRiddle(Riddle riddle);
    int removeRiddle(int id);
    Riddle getRiddle(int id);
    std::map<int, Riddle> getAllRiddles();
    int updateRiddle(Riddle riddle);
    void replaceRiddles(std::map<int, Riddle> riddles);
    void loadBaseFromDisk(std::string path);
    void saveBaseOnDisk(std::string path);
    int getRiddlesAmount();
};


#endif //JZDNS_RIDDLEBASE_H
