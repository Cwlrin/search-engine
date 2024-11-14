#include "Dictionary.h"

#include <iostream>

void readFile(string filename) {
  std::ifstream ifs(filename);
  if (!ifs) {
    cout << "open file error" << "\n";
    return;
  }

  std::string line;
  while (std::getline(ifs, line)) {
    for (auto &c : line) {
      c = tolower(c);
      if ((isalpha(c) == 0) || c == ' ') c = ' ';
    }
    std::cout << line << std::endl;
  }
  ifs.close();
}

int main() {
  readFile("/home/ichika/SearchEngine/res/yuliao/english.txt");
  return 0;
}