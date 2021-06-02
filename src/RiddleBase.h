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

class RiddleBase {
private:
    std::map<uint64_t, Riddle> riddles;
    template<class Archive>
    void serialize(Archive &ar, const unsigned int version)
    {
        ar & BOOST_SERIALIZATION_NVP(riddles);
    }
public:
    uint64_t addRiddle(Riddle riddle);
    int removeRiddle(uint64_t id);
    Riddle* getRiddle(uint64_t id);
    std::map<uint64_t, Riddle> getAllRiddles();
    int updateRiddle(Riddle riddle);
    void loadBaseFromDisk(std::string path);
    void saveBaseOnDisk(std::string path);
    int getRiddlesAmount();
};


#endif //JZDNS_RIDDLEBASE_H
