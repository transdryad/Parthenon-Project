#include <iostream>
#include "src/question.hpp"

int ask(Question question) {
    std::cout << question.question << std::endl;
    std::cout << "1: " << question.answers[0] << std::endl;
    std::cout << "2: " << question.answers[1] << std::endl;
    std::cout << "3: " << question.answers[2] << std::endl;
    std::cout << "4: " << question.answers[3] << std::endl;
    std::cout << "Your Answer: ";
    int answer;
    std::cin >> answer;
    std::cout << std::endl;
    return answer;
}

int main() {
    std::cout << "Hello World" << std::endl;
    


    return 0;
}
