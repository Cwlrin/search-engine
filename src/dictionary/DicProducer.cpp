#include "DicProducer.h"

DictProducer::DictProducer(const string& dir) { readFile(dir); }

DictProducer::DictProducer(const string& dir, SplitTool* splitTool) {}

void DictProducer::buildEnDict() {}

void DictProducer::bulidCnDict() {}

void DictProducer::createIndex() {}

void DictProducer::store() {}

void DictProducer::readFile(const string& dir) {
  std::ifstream ifs(dir);
  if (!ifs) {
    cout << "open file" << dir << "error" << "\n";
    return;
  }

  std::ofstream ofs("/home/ichika/SearchEngine/res/temp/cleanedTextEn.txt");
  if (!ofs) {
    cout << "open file error" << "\n";
  }

  string line;
  while (std::getline(ifs, line)) {
    string cleanedLine = cleanText(line);
    if (!cleanedLine.empty()) {
      ofs << cleanedLine << "\n";
    }
  }
}

string DictProducer::cleanText(const string& lines) {
  string lowerText;
  // 大写转小写
  std::transform(lines.begin(), lines.end(), std::back_inserter(lowerText),
                 [](unsigned char c) { return tolower(c); });

  // 去除所有非小写字母的字符
  std::regex nonLetters("[^a-z ]");
  std::string cleanedText = std::regex_replace(lowerText, nonLetters, " ");

  // 去除连续空格字符
  regex multipleSpaces("\\s+");
  cleanedText = regex_replace(cleanedText, multipleSpaces, " ");

  // 去除字符串开头和结尾的空格
  size_t start = cleanedText.find_first_not_of(" ");
  size_t end = cleanedText.find_last_not_of(" ");
  if (start == string::npos || end == string::npos) {
    return "";  // 如果字符串全是空格，则返回空字符串
  }
  return cleanedText.substr(start, end - start + 1);
}
