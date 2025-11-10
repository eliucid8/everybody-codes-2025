#include "utils.hpp"
#include <cmath>
#include <iostream>
#include <string>

using namespace std;

std::string part1(std::vector<std::string> input) {
    int start = tokenize_int(input[0], "|")[0];
    int end = tokenize_int(input[input.size() - 1], "|")[1];
    return std::to_string(2025 * start / end);
}

std::string part2(std::vector<std::string> input) {
    int start = tokenize_int(input[0], "|")[0];
    int end = tokenize_int(input[input.size() - 1], "|")[1];
    return std::to_string(10000000000000 * end / start);
}

std::string part3(std::vector<std::string> input) {
    int start = tokenize_double(input[0], "|")[0];
    double ratio = start;
    for(int i = 1; i < input.size() - 1; i++) {
        auto shaft = tokenize_double(input[i], "|");
        ratio /= shaft[0];
        ratio *= shaft[1];
    }
    ratio /= tokenize_double(input[input.size()-1], "|")[0];

    return std::to_string(std::floor(100 * ratio));
}

int main() {
    std::string ans1 = part1(read_lines("days/day04/input.txt"));
    std::cout << "Answer 1: " << ans1 << std::endl;
    std::string ans2 = part2(read_lines("days/day04/input2.txt"));
    std::cout << "Answer 2: " << ans2 << std::endl;
    std::string ans3 = part3(read_lines("days/day04/input3.txt"));
    std::cout << "Answer 3: " << ans3 << std::endl;
}
