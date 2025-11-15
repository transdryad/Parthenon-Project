#include "src/question.hpp"

Question::Question(std::string question, std::string answers[4], int correct) {
    this->question = question;
    this->answers = answers;
    this->correct = correct;
}
