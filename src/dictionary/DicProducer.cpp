#include "DicProducer.h"

DictProducer::DictProducer(const string &dir) {
  ifstream configFile(dir);
  if (!configFile.is_open()) {
    throw std::runtime_error("Failed to open configuration file.");
  }
  json configJson = json::parse(configFile);

  // 读取英文语料库路径
  string englishPath = configJson["corpus_dirs"]["english"];
  for (const auto &entry : fs::directory_iterator(englishPath)) {
    if (entry.is_regular_file()) {
      _files.push_back(entry.path().string());
    }
  }
}

DictProducer::DictProducer(const string &dir, SplitTool *splitTool) {
  std::ifstream configFile(dir);
  if (!configFile.is_open()) {
    throw std::runtime_error("Failed to open configuration file.");
  }
  json configJson = json::parse(configFile);

  // 读取中文语料库路径
  string chinesePath = configJson["corpus_dirs"]["cn_corpus"];
  for (const auto &entry : fs::directory_iterator(chinesePath)) {
    if (entry.is_regular_file()) {
      _files.push_back(entry.path().string());
    }
  }
}

void DictProducer::buildEnDict() {
  ifstream cleanedFile("res/temp/cleanedTextEn.txt");

  if (!cleanedFile) {
    cerr << "Failed to open cleaned corpus file for dictionary construction." << endl;
    return;
  }

  ifstream stopWordsFile("res/yuliao/stop_words_eng.txt");
  if (!stopWordsFile) {
    cerr << "Failed to open stop words file for filtering." << endl;
    return;
  }

  // 将停用词加载到哈希集合 std::unordered_set 中供快速查找
  std::unordered_set<string> stopWords;
  string stopWord;
  while (stopWordsFile >> stopWord) {
    stopWords.insert(stopWord);
  }

  string line;
  while (std::getline(cleanedFile, line)) {
    istringstream iss(line);
    string word;

    while (iss >> word) {
      if (stopWords.count(word)) {
        continue;  // 忽略停用词
      }

      auto it = std::find_if(_dict.begin(), _dict.end(),
                             [&word](const pair<string, int> &p) {
                               return p.first == word;
                             });
      if (it != _dict.end()) {
        it->second++;  // 增加词频
      } else {
        _dict.emplace_back(word, 1);
      }
    }
  }

  // 对词典按词频降序、字母升序排序
  std::sort(_dict.begin(), _dict.end(),
            [](const pair<string, int> &a, const pair<string, int> &b) {
              if (a.second != b.second) {
                return a.second > b.second;  // 词频降序
              }
              return a.first < b.first;  // 词频相同时按字母升序
            });

  cout << "English dictionary built with " << _dict.size() << " unique words." << endl;
}

void DictProducer::buildCnDict() {}

void DictProducer::createIndex() {}

void DictProducer::store() {
  // 导出英文词典
  const string outputPath = "res/EnDict.dat";
  ofstream ofs(outputPath);

  if (!ofs) {
    cerr << "Failed to open output file: " << outputPath << endl;
    return;
  }

  for (const auto &entry : _dict) {
    ofs << entry.first << " " << entry.second << "\n";  // 格式化输出：单词 词频
  }

  cout << "Dictionary successfully saved to " << outputPath << endl;

  // 清理临时文件
  const string tempDir = "res/temp";

  try {
    for (const auto &entry : fs::directory_iterator(tempDir)) {
      if (entry.is_regular_file()) {
        fs::remove(entry.path());
        cout << "Deleted temporary file: " << entry.path() << endl;
      }
    }
  } catch (const std::exception &e) {
    cerr << "Error cleaning temporary files: " << e.what() << endl;
  }
}

void DictProducer::readFile(const string &dir) {
  static const string tempFilePath = "res/temp/cleanedTextEn.txt";

  ifstream ifs(dir);
  if (!ifs) {
    cerr << "Failed to open file: " << dir << endl;
    return;
  }

  ofstream ofs(tempFilePath, std::ios::app);  // 追加模式打开临时文件
  if (!ofs) {
    cerr << "Failed to open temporary cleaned file: " << tempFilePath << endl;
    return;
  }

  string line;
  while (std::getline(ifs, line)) {
    string cleanedLine = cleanText(line);
    if (!cleanedLine.empty()) {
      ofs << cleanedLine << "\n";
    }
  }
  cout << "Cleaned content from file: " << dir << " appended to "
       << tempFilePath << endl;
}

string DictProducer::cleanText(const string &lines) {
  string lowerText;
  // 大写转小写
  std::transform(lines.begin(), lines.end(), std::back_inserter(lowerText),
                 [](unsigned char c) { return tolower(c); });

  // 去除所有非小写字母的字符
  regex nonLetters("[^a-z ]");
  string cleanedText = std::regex_replace(lowerText, nonLetters, " ");

  // 去除连续空格字符
  regex multipleSpaces("\\s+");
  cleanedText = regex_replace(cleanedText, multipleSpaces, " ");

  // 去除字符串开头和结尾的空格
  size_t start = cleanedText.find_first_not_of(' ');
  size_t end = cleanedText.find_last_not_of(' ');
  if (start == string::npos || end == string::npos) {
    return "";  // 如果字符串全是空格，则返回空字符串
  }
  return cleanedText.substr(start, end - start + 1);
}

[[maybe_unused]] void DictProducer::getFileName(const string &path) {
  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      _files.push_back(entry.path().string());
    }
  }
}