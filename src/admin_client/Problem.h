//
// Author: Artur Mazur
//

#ifndef PROBLEM_H
#define PROBLEM_H

#include <string>
#include <iostream>

class Problem
{
    private:
        int index;
        std::string question;
        std::string answer;

    public:
        Problem(int i, std::string q, std::string a);

        int getIndex();

        std::string getQuestion();

        std::string getAnswer();

        void printInfo();

        void setQuestion(std::string q);

        void setAnswer(std::string a);

};


#endif
