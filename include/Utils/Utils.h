#pragma once

#include <string>
#include <sstream>
#include <vector>

#define SE_GET_TYPE_NAME(type) #type

void split(const std::string &s, char delim, std::back_insert_iterator<std::vector<std::string>> result);
std::vector<std::string> split(const std::string &s, char delim);
void replaceAll(std::string& str, const std::string& from, const std::string& to);
void removeSpaces(std::string& str);