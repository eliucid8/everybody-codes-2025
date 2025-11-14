#include "utils.hpp"
#include <cctype>
#include <cstdint>
#include <iostream>
#include <map>

using namespace std;

typedef uint64_t ull;

std::string part1(std::vector<std::string> input) {
    string knights = input[0];
    int num_mentors = 0;
    int total_pairs = 0;
    for(int i = 0; i < knights.size(); i++) {
        if(knights[i] == 'A') {
            num_mentors++;
        }
        if(knights[i] == 'a') {
            total_pairs += num_mentors;
        }
    }
    return to_string(total_pairs);
}

std::string part2(std::vector<std::string> input) {
    string knights = input[0];
    int a_mentors = 0;
    int a_pairs = 0;
    int b_mentors = 0;
    int b_pairs = 0;
    int c_mentors = 0;
    int c_pairs = 0;
    for(int i = 0; i < knights.size(); i++) {
        if(knights[i] == 'A') {
            a_mentors++;
        }
        if(knights[i] == 'a') {
            a_pairs += a_mentors;
        }
        if(knights[i] == 'B') {
            b_mentors++;
        }
        if(knights[i] == 'b') {
            b_pairs += b_mentors;
        }
        if(knights[i] == 'C') {
            c_mentors++;
        }
        if(knights[i] == 'c') {
            c_pairs += c_mentors;
        }
    }
    return to_string(a_pairs + b_pairs + c_pairs);
}


std::string part3(std::vector<std::string> input, int range, int repeats) {
    string knights = input[0];
    // 10000 tents long.
    // cout << knights.size() << ", " << knights.length();
    
    map<char, int> mentors;
    map<char, int> pairs;
    vector<char> mentor_chars = {'A', 'B', 'C'};
    vector<char> trainee_chars = {'a', 'b', 'c'};

    const int RANGE = range;
    const int REPEATS = repeats;

// wraparound case: initialize the mentor cycle
    for(int i = -RANGE; i <= RANGE; i++) {
        int index = i < 0 ? knights.size() + i : i;
        if(isupper(knights[index])) {
            mentors[knights[index]]++;
        }
    }
    //
    // for(auto item : mentors) {
    //     cout << item.first << ", "<< item.second << endl;
    // }

    for(int i = 0; i < knights.length(); i++) {
        // if trainee, add current mentors.
        if(islower(knights[i])) {
            pairs[knights[i]] += mentors[toupper(knights[i])];
        }
        // if lbound out of range, wrap around.
        int lbound = i - RANGE; // in window, about to leave window
        lbound = lbound < 0 ? knights.length() + lbound : lbound;
        int ubound = i + RANGE + 1; // out of window, about to enter window
        ubound = ubound >= knights.length() ? ubound - knights.length() : ubound;

        if(isupper(knights[lbound])) {
            mentors[knights[lbound]]--;
        }
        if(isupper(knights[ubound])) {
            mentors[knights[ubound]]++;
        }
    }

    int total = 0;
    for(auto item : pairs) {
        // cout << item.first << ": "<< item.second << endl;
        total += item.second;
    }
    total *= max(REPEATS - 1, 0);
    // cout << "wrapped total: " << total << endl;

    // it's just equivalent to doing repeats-1 wrapped calcs and 1 fully bounded calc.
    // originally I bounded on left and bounded on right, but we're just combining the bounded halves together, because RANGE < knights.length() / 2

    // bounded.
    mentors.clear();
    pairs.clear();

    // sum the mentors for only the positive values
    for(int i = 0; i <= RANGE; i++) {
        int index = i < 0 ? knights.size() + i : i;
        if(isupper(knights[index])) {
            mentors[knights[index]]++;
        }
    }
    for(int i = 0; i < knights.length(); i++) {
        // if trainee, add current mentors.
        if(islower(knights[i])) {
            pairs[knights[i]] += mentors[toupper(knights[i])];
            // cout << "add " << mentors[toupper(knights[i])] << endl;
        }
        // if lbound out of range, wrap around.
        int lbound = i - RANGE; // in window, about to leave window
        if(lbound >= 0 && isupper(knights[lbound])) {
            mentors[knights[lbound]]--;
        }

        int ubound = i + RANGE + 1; // out of window, about to enter window
        if(ubound < knights.length() && isupper(knights[ubound])) {
            mentors[knights[ubound]]++;
        }
    }
    int left_bounded = pairs['a'] + pairs['b'] + pairs['c'];
    // cout << "bounded: " << left_bounded << endl;

    total += left_bounded;

    return to_string(total);
}

int main() {
    std::string ans1 = part1(read_lines("days/day06/input.txt"));
    std::cout << "Answer 1: " << ans1 << std::endl;
    std::string ans2 = part2(read_lines("days/day06/input2.txt"));
    std::cout << "Answer 2: " << ans2 << std::endl;
    std::string test = part3(read_lines("days/day06/test_input3.txt"), 10, 1);
    std::cout << "Test 3: " << test << std::endl;
    std::string ans3 = part3(read_lines("days/day06/input3.txt"), 1000, 1000);
    std::cout << "Answer 3: " << ans3 << std::endl;
}
