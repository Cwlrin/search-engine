#ifndef _DICTIONARY_H_
#define _DICTIONARY_H_

#include <algorithm>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <map>
#include <nlohmann/json.hpp>
#include <regex>
#include <set>
#include <string>
#include <unordered_set>
#include <vector>

using std::back_inserter;
using std::cerr;
using std::cout;
using std::endl;
using std::exception;
using std::find_if;
using std::getline;
using std::ifstream;
using std::ios;
using std::istringstream;
using std::map;
using std::ofstream;
using std::pair;
using std::regex;
using std::runtime_error;
using std::set;
using std::sort;
using std::string;
using std::transform;
using std::unordered_set;
using std::unordered_map;
using std::vector;

using json = nlohmann::json;

namespace fs = std::filesystem;

#endif  // _DICTIONARY_H_