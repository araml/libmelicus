#include <vector>
#include <string>
#include <sstream>

std::vector<std::string> split_string(std::string line) {
    std::stringstream ss(line);
    std::string word;
    std::vector<std::string> words;
    while (std::getline(ss, word, '\n')) {
        words.emplace_back(word);
    }

    return words;
}
