#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <string>

std::string part1(std::string path) {
    std::vector<std::string> lines = read_lines(path);
    auto names = tokenize_string(lines[0], ",");
    auto instrs = tokenize_string(lines[2], ",");
    int i = 0;
    for(std::string instr : instrs) {
        // auto foo = instr.substr(1);
        // std::cout << foo << std::endl;

        int dist = std::stoi(instr.substr(1));
        if(instr.at(0) == 'L') {
            dist = -dist;
        }
        i = std::clamp<int>(i + dist, 0, names.size()-1);
        // std::cout << names[i] << std::endl;
    }

    return names[i];
}

std::string part2(std::string path) {
    std::vector<std::string> lines = read_lines(path);
    auto names = tokenize_string(lines[0], ",");
    auto instrs = tokenize_string(lines[2], ",");

    int i = 0;
    for(std::string instr : instrs) {
        // std::cout << instr << std::endl;

        int dist = std::stoi(instr.substr(1));
        if(instr.at(0) == 'L') {
            dist = -dist;
        }
        i += dist;
        
        // if(i < 0) {
        //     i += names.size();
        // }
        // std::cout << i << " " << names[i] << std::endl;
    }

    i %= names.size();
    return names[i];
}

std::string part3(std::string path) {
    std::vector<std::string> lines = read_lines(path);
    auto names = tokenize_string(lines[0], ",");
    auto instrs = tokenize_string(lines[2], ",");

    for(std::string instr : instrs) {
        int i = std::stoi(instr.substr(1));
        if(instr.at(0) == 'L') {
            i = -i;
        }
        i %= names.size();
        if(i < 0) {
            i += names.size();
        }

        std::swap(names[0], names[i]);

        // for(auto name : names) {
        //     std::cout << name;    
        // }
        // std::cout << std::endl;
    }

    return names[0];
}

int main() {
    std::string ans1 = part1("days/day01/input.txt");
    std::cout << "Part 1: " << ans1 << std::endl;
    std::string ans2 = part2("days/day01/input2.txt");
    std::cout << "Part 2: " << ans2 << std::endl;
    std::string ans3 = part3("days/day01/input3.txt");
    std::cout << "Part 3: " << ans3 << std::endl;
}
