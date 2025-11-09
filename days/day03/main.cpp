#include "utils.hpp"
#include <algorithm>
#include <iostream>
#include <string>
#include <unordered_set>
#include <set>
#include <unordered_map>

std::string part1(std::vector<std::string> input) {
    auto boxes = tokenize_int(input[0], ",");
    std::unordered_set<int> uniq_boxes;
    for(int box : boxes) {
        uniq_boxes.insert(box);
    }

    // each box can either fit into a box or fit another box inside of it. 
    // The only boxes we need to exclude are the duplicate numbers
    int total = 0;
    for(int box : uniq_boxes) {
        total += box;
    }
    return std::to_string(total);
}

std::string part2(std::vector<std::string> input) {
    auto boxes = tokenize_int(input[0], ",");
    std::set<int> uniq_boxes;
    for(int box : boxes) {
        uniq_boxes.insert(box);
    }

    int total = 0;
    int index = 0;
    for(auto box : uniq_boxes) {
        if(index == 20) {
            break;
        }
        total += box;
        index++;
    }

    return std::to_string(total);
}

// the only thing keeping us from putting two sets of boxes together are the duplicate numbers.
// so we just need to find the max number of duplicates.
std::string part3(std::vector<std::string> input) {
    auto boxes = tokenize_int(input[0], ",");
    std::unordered_map<int, int> box_counter;
    int max_count = 0;
    for(int box : boxes) {
        box_counter[box]++;
        max_count = std::max(max_count, box_counter[box]);
    }

    return std::to_string(max_count);
}


int main() {
    std::string ans1 = part1(read_lines("days/day03/input.txt"));
    std::cout << "Answer 1: " << ans1 << std::endl;
    std::string ans2 = part2(read_lines("days/day03/input2.txt"));
    std::cout << "Answer 2: " << ans2 << std::endl;
    std::string ans3 = part3(read_lines("days/day03/input3.txt"));
    std::cout << "Answer 3: " << ans3 << std::endl;
}
