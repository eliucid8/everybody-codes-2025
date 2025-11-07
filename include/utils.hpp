#pragma once
#include <string>
#include <vector>

std::vector<std::string> read_lines(const std::string& filename);

std::vector<std::string> tokenize_string(const std::string& s, const std::string& delimiter);
std::vector<int> tokenize_int(const std::string& s, const std::string& delimiter);

