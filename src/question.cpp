#include "src/question.hpp"


Question::Question(std::string question, std::string answers[NUM_QUESTIONS], int correct) {
    this->question = question;
    for (size_t i = 0; i < NUM_QUESTIONS; i++) {
        this->answers[i] = answers[i];
    }
    this->correct = correct;
}
