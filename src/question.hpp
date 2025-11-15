#pragma once

#include <string>

#define NUM_QUESTIONS 4

class Question {
    public:
        Question(std::string question, std::string answers[NUM_QUESTIONS], int correct);

        std::string question;
        std::string answers[4];
        int correct;
};
