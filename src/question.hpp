#pragma once

#include <string>

class Question {
    public:
        Question(std::string question, std::string answers[4], int correct);

        std::string question;
        std::string answers[4];
        int correct;
};
