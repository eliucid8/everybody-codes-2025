#include "utils.hpp"
#include <iostream>
#include <regex>
#include <format>
#include <cstdint>

typedef std::pair<int, int> pii;
typedef std::pair<int64_t, int64_t> pll;

pii parse_input(std::string complex) {
    const std::regex r(R"raw(\[(-?\d+),(-?\d+)])raw");
    const std::vector<std::smatch> matches{
        std::sregex_iterator(complex.begin(), complex.end(), r),
        std::sregex_iterator{}
    };
    std::string c1 = matches[0].str(1);
    std::string c2 = matches[0].str(2);
    return pii(std::stoi(c1), std::stoi(c2));
}

std::string part1(std::vector<std::string> lines) {
    auto a = parse_input(lines[0]);
    auto r = pii(0, 0);
    for(int i = 0; i < 3; i++) {
        r = pii(r.first * r.first - r.second * r.second, r.first * r.second + r.first * r.second);
        // std::cout << std::format("[{},{}]", r.first, r.second) << std::endl;
        r = pii(r.first / 10, r.second / 10);
        // std::cout << std::format("[{},{}]", r.first, r.second) << std::endl;
        r = pii(r.first + a.first, r.second + a.second);
        // std::cout << std::format("[{},{}]", r.first, r.second) << std::endl;
    }
    return std::format("[{},{}]", r.first, r.second);
}

pll iterate(pll r, pll start) {
    r = pll(r.first * r.first - r.second * r.second, 2 * r.first * r.second);
    r = pll(r.first / 100000, r.second / 100000);
    r = pll(r.first + start.first, r.second + start.second);
    return r;
}

std::string part2(std::vector<std::string> lines) {
    auto startii = parse_input(lines[0]);
    pll start = pll(startii.first, startii.second);

    std::vector<std::vector<bool>> grid(101, std::vector<bool>(101, false));

    int successes = 0;
    for(int x = 0; x <= 1000; x+=10) {
        for(int y = 0; y <= 1000; y+=10) {
            pll r = pll(0,0);
            pll a = pll(startii.first + x, startii.second + y);
            bool success = true;
            for(int i = 0; i < 100; i++) {
                // if (x == 0 && y == 0) {
                //     std::cout << std::format("[{},{}]", r.first, r.second) << std::endl;
                // }
                r = iterate(r, a);
                if(std::abs(r.first) > 1000000 || std::abs(r.second) > 1000000) {
                    success = false;
                    break;
                }
            }
            if(success) {
                successes++;
            }
            grid[x/10][y/10] = success;
        }
    }

    for(int y = 0; y < 101; y++) {
        for(int x = 0; x < 101; x++) {
            if(grid[x][y]) {
                std::cout << "x";
            } else {
                std::cout << ".";
            }
        }
        std::cout << std::endl;
    }

    return std::to_string(successes);
}

std::string part3(std::vector<std::string> lines) {
    auto startii = parse_input(lines[0]);
    pll start = pll(startii.first, startii.second);

    std::vector<std::vector<bool>> grid(1001, std::vector<bool>(1001, false));

    int successes = 0;
    for(int x = 0; x <= 1000; x+=1) {
        for(int y = 0; y <= 1000; y+=1) {
            pll r = pll(0,0);
            pll a = pll(startii.first + x, startii.second + y);
            bool success = true;
            for(int i = 0; i < 100; i++) {
                r = iterate(r, a);
                if(std::abs(r.first) > 1000000 || std::abs(r.second) > 1000000) {
                    success = false;
                    break;
                }
            }
            if(success) {
                successes++;
            }
            grid[x][y] = success;
        }
    }
    return std::to_string(successes);
}

int main() {
    std::string ans1 = part1(read_lines("days/day02/input.txt"));
    std::cout << "Answer 1:" << ans1 << std::endl;
    std::string ans2 = part2(read_lines("days/day02/input2.txt"));
    std::cout << "Answer 2:" << ans2 << std::endl;
    std::string ans3 = part3(read_lines("days/day02/input3.txt"));
    std::cout << "Answer 3:" << ans3 << std::endl;
}
