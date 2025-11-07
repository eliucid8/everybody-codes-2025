#include "utils.hpp"
#include <any>
#include <fstream>
#include <string>
#include <vector>

std::vector<std::string> read_lines(const std::string& filename) {
    std::vector<std::string> lines;
    std::ifstream file(filename);
    std::string line;
    while (std::getline(file, line)) {
        lines.push_back(line);
    }
    return lines;
}

std::vector<std::string> tokenize_string(const std::string& s, const std::string& delimiter) {
    std::vector<std::string> tokens;
    size_t pos = 0;
    std::string temp_s = s; // Create a mutable copy

    while ((pos = temp_s.find(delimiter)) != std::string::npos) {
        tokens.push_back(temp_s.substr(0, pos));
        temp_s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(std::any_cast<std::string>(temp_s)); // Add the last token

    return tokens;
}


std::vector<int> tokenize_int(const std::string& s, const std::string& delimiter) {
    std::vector<int> tokens;
    size_t pos = 0;
    std::string temp_s = s; // Create a mutable copy

    while ((pos = temp_s.find(delimiter)) != std::string::npos) {
        tokens.push_back(std::stoi(temp_s.substr(0, pos)));
        temp_s.erase(0, pos + delimiter.length());
    }
    tokens.push_back(std::any_cast<int>(temp_s)); // Add the last token

    return tokens;
}

// std::vector<int> tokenize(const std::string& s, const std::string& delimiter) {
//     std::vector<int> tokens;
//     size_t pos = 0;
//     std::string temp_s = s; // Create a mutable copy
//
//     while ((pos = temp_s.find(delimiter)) != std::string::npos) {
//         tokens.push_back(std::any_cast<int>(temp_s.substr(0, pos)));
//         temp_s.erase(0, pos + delimiter.length());
//     }
//     tokens.push_back(temp_s); // Add the last token
//
//     return tokens;
// }
