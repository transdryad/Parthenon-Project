#include <iostream>
#include <vector>
#include <optional>
#include "src/question.hpp"
#include "src/toml.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "A question file is required" << std::endl;
        return 1;
    }

    toml::table tbl;
    std::vector<Question> questions;

    try //parse toml into memory
    {
        tbl = toml::parse_file(argv[1]);
    }
    catch (const toml::parse_error& err)
    {
        std::cerr << "Parsing failed:\n" << err << "\n";
        return 1;
    }

    //std::cout << tbl["questions"] << std::endl;
    for (size_t i = 0; i < tbl["questions"].as_array()->size(); i++) { //put questions in our nice data structure
        std::optional<std::string> question = tbl["questions"][i][0].value<std::string>();
        std::optional<int> correct = tbl["questions"][i][2].value<int>();
        std::string answers[4];
        for (int j = 0; j < 4; j++) {
            std::optional<std::string> thing = tbl["questions"][i][1][j].value<std::string>();
            answers[j] = thing.value_or("");
        }
        
        questions.emplace_back(question.value_or(""), answers, correct.value_or(0));
    }

    for (auto q : questions) {
        std::cout << q.question << ":";
        for (auto a : q.answers) { std::cout << " " << a; }
        std::cout << ". " << q.correct << std::endl;
    }

    return 0;
}
