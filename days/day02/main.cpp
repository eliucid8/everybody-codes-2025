#include "utils.hpp"
#include <iostream>

int main() {
    auto lines = read_lines("days/day02/input.txt");

    std::cout << "Line count: " << lines.size() << std::endl;
}
