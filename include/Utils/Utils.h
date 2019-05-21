#pragma once

#include <string>
#include <sstream>
#include <vector>

void split(const std::string &s, char delim, std::back_insert_iterator<std::vector<std::string>> result);

std::vector<std::string> split(const std::string &s, char delim);

void replaceAll(std::string& str, const std::string& from, const std::string& to);