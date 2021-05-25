//author: Patryk Karbownik
#ifndef JZDNS_RIDDLE_H
#define JZDNS_RIDDLE_H

#include <boost/serialization/access.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "string"

class Riddle {
private:
    uint64_t id;
    std::string riddleContent;
    std::string answer;
    friend class boost::serialization::access;
    template<class Archive>
    void serialize(Archive & archive, const unsigned int version)
    {
        archive & BOOST_SERIALIZATION_NVP(id);
        archive & BOOST_SERIALIZATION_NVP(riddleContent);
        archive & BOOST_SERIALIZATION_NVP(answer);
    }
public:
    Riddle();
    Riddle(uint64_t id, std::string riddleContent, std::string answer);
    uint64_t getId();
    std::string getRiddleContent();
    std::string getAnswer();
    void setId(uint64_t id);
    void setRiddleContent(std::string riddleContent);
    void setAnswer(std::string answer);
    bool operator==(const Riddle &rhs) const;
    bool operator!=(const Riddle &rhs) const;
};


#endif //JZDNS_RIDDLE_H