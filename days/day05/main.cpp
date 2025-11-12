#include "utils.hpp"
#include <cstdint>
#include <iostream>
#include <optional>
#include <regex>

using namespace std;

typedef pair<int, vector<int>> piv;
typedef tuple<optional<int>, int, optional<int>> t3i;

class Sword {
public:
    Sword(piv list) {
        id = list.first;
        t3i current_level;
        for(int num : list.second) {
            if(fishbone.empty()){
                fishbone.push_back(t3i(nullopt, num, nullopt));
                continue;
            }

            bool placed = false;
            // for(auto segment : fishbone) {
            for(int i = 0; i < fishbone.size(); i++) {
                auto segment = fishbone[i];
                if(num < get<1>(segment)) {
                    if(!get<0>(segment).has_value()) {
                        get<0>(segment) = make_optional(num);
                        placed = true;
                        fishbone[i] = segment;
                        break;
                    }
                } else if(num > get<1>(segment)) {
                    if(!get<2>(segment).has_value()) {
                        get<2>(segment) = make_optional(num);
                        placed = true;
                        fishbone[i] = segment;
                        break;
                    }
                }
            }
            if(!placed) {
                fishbone.push_back(t3i(nullopt, num, nullopt));
            }
        }

        calculate_levels();
    }

    string to_string() {
        string out = "";
        // string out = std::to_string(id);
        // out += "\n";
        for(t3i level : fishbone) {
            if(get<0>(level).has_value()) {
                out += std::to_string(get<0>(level).value());
                out += "-";
            } else {
                out += "  ";
            }
            out += std::to_string(get<1>(level));
            if(get<2>(level).has_value()) {
                out += "-";
                out += std::to_string(get<2>(level).value());
            }
            out +="\n  |\n";
        }
        return out;
    }

    string quality() {
        string out = "";
        for(auto segment : fishbone) {
            out += std::to_string(get<1>(segment));
        }
        return out;
    }

    uint64_t quality_ull() {
        return stoull(quality());
    }

    void calculate_levels() {
        levels.clear();
        for(auto segment : fishbone) {
            int level_number = 0;
            if(get<0>(segment).has_value()) {
                // level_number *= 10;
                level_number += get<0>(segment).value();
            }
            level_number *= 10;
            level_number += get<1>(segment);
            if(get<2>(segment).has_value()) {
                level_number *= 10;
                level_number += get<2>(segment).value();
            }
            levels.push_back(level_number);
        }
    }

    bool operator<(Sword& other) {
        return compare(other);
    }

    bool compare(Sword& other){
        if(this->quality_ull() != other.quality_ull()) {
            return this->quality_ull() < other.quality_ull();
        }
        // necessarily they must have the same number of levels
        for(int i = 0; i < levels.size(); i++) {
            if(this->levels[i] != other.levels[i]) {
                return this->levels[i] < other.levels[i];
            }
        }
        return this->id < other.id;
    }

    int id;
    vector<t3i> fishbone;
    vector<int> levels;
};

// return pair of id, list of comma-delimited values afterwards
piv parse_input(std::string input) {
    auto id_split = tokenize_string(input, ":");
    int id = stoi(id_split[0]);
    auto values = tokenize_int(id_split[1], ",");
    return piv(id, values);
}

std::string part1(std::vector<std::string> input) {
    auto intermediate = parse_input(input[0]);
    Sword sword = Sword(intermediate);
    return sword.quality();
}

std::string part2(std::vector<std::string> input) {
    uint64_t min_qual;
    uint64_t max_qual;


    for(string line : input) {
        auto intermediate = parse_input(line);
        Sword sword = Sword(intermediate);
        uint64_t quality = stoull(sword.quality());
        // cout << quality << endl;
        max_qual = std::max(quality, max_qual);
        if(min_qual != 0){
            min_qual = std::min(quality, min_qual);
        } else {
            min_qual = quality;
        }
        // cout << min_qual << ", " << max_qual << endl;
    }

    return std::to_string(max_qual - min_qual);
}

std::string part3(std::vector<std::string> input) {
    vector<Sword> swords;
    for(string line : input) {
        auto intermediate = parse_input(line);
        Sword sword = Sword(intermediate);
        swords.push_back(sword);
    }

    std::sort(swords.begin(), swords.end());

    int checksum = 0;
    for(int i = 0; i < swords.size(); i++) {
        checksum += swords[i].id * (swords.size() - i);
    }
    return std::to_string(checksum);
}

int main() {
    std::string ans1 = part1(read_lines("days/day05/input.txt"));
    std::cout << "Answer 1: " << ans1 << std::endl;
    std::string ans2 = part2(read_lines("days/day05/input2.txt"));
    std::cout << "Answer 2: " << ans2 << std::endl;
    std::string ans3 = part3(read_lines("days/day05/input3.txt"));
    std::cout << "Answer 3: " << ans3 << std::endl;
}
