#include "dictionary/DicProducer.h"

DictProducer::DictProducer(const string &dir) {
  ifstream config_file(dir);
  if (!config_file.is_open()) {
    throw std::runtime_error("Failed to open configuration file.");
  }
  json config_json = json::parse(config_file);

  // 读取英文语料库路径
  string english_path = config_json["corpus_dirs"]["english"];
  for (const auto &entry : fs::directory_iterator(english_path)) {
    if (entry.is_regular_file()) {
      files_.push_back(entry.path().string());
    }
  }
}

DictProducer::DictProducer(const string &dir, SplitTool *split_tool)
    : cuttor_(split_tool) {
  if (!split_tool) {
    throw std::runtime_error(
        "SplitTool is required for Chinese dictionary generation.");
  }

  ifstream config_file(dir);
  if (!config_file.is_open()) {
    throw std::runtime_error("Failed to open configuration file.");
  }
  json config_json = json::parse(config_file);

  // 读取中文语料库路径
  string chinese_path = config_json["corpus_dirs"]["chinese"];
  for (const auto &entry : fs::directory_iterator(chinese_path)) {
    if (entry.is_regular_file()) {
      files_.push_back(entry.path().string());
    }
  }
}

void DictProducer::BuildEnDict() {
  ifstream cleaned_file("res/temp/cleanedTextEn.txt");

  if (!cleaned_file) {
    cerr << "Failed to open cleaned corpus file for dictionary construction."
         << endl;
    return;
  }

  ifstream stop_words_file("res/yuliao/stop_words_eng.txt");
  if (!stop_words_file) {
    cerr << "Failed to open stop words file for filtering." << endl;
    return;
  }

  // 将停用词加载到哈希集合 std::unordered_set 中供快速查找
  std::unordered_set<string> stop_words;
  string stop_word;
  while (stop_words_file >> stop_word) {
    stop_words.insert(stop_word);
  }

  string line;
  while (std::getline(cleaned_file, line)) {
    istringstream iss(line);
    string word;

    while (iss >> word) {
      if (stop_words.count(word)) {
        continue;  // 忽略停用词
      }

      auto it = std::find_if(
          dict_.begin(), dict_.end(),
          [&word](const pair<string, int> &p) { return p.first == word; });
      if (it != dict_.end()) {
        it->second++;  // 增加词频
      } else {
        dict_.emplace_back(word, 1);
      }
    }
  }

  // 对词典按词频降序、字母升序排序
  std::sort(dict_.begin(), dict_.end(),
            [](const pair<string, int> &a, const pair<string, int> &b) {
              if (a.second != b.second) {
                return a.second > b.second;  // 词频降序
              }
              return a.first < b.first;  // 词频相同时按字母升序
            });

  cout << "English dictionary built with " << dict_.size() << " unique words."
       << endl;
}

// 检查是否是中文字符
bool is_chinese_char(const uint32_t codepoint) {
  return codepoint >= 0x4E00 && codepoint <= 0x9FA5;  // 中文字符范围
}

// 检查是否是英文字母
bool is_english_char(const unsigned char c) {
  return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');  // 英文字母范围
}

// 检查是否是标点符号（中英文符号）
bool is_punctuation(const unsigned char c) {
  // 常见英文标点和部分中文标点
  const std::string punctuations =
      ".,!?;:\"'()[]{}<>~`@#$%^&*-+=_|\\/，。！？；：“”‘’（）【】《》……";
  return punctuations.find(c) != std::string::npos;
}

void DictProducer::BuildCnDict() {
  static const string temp_file_path = "res/temp/cleanedTextCn.txt";

  ifstream cleaned_file(temp_file_path);
  if (!cleaned_file) {
    cerr << "Failed to open cleaned corpus file for dictionary construction."
         << endl;
    return;
  }

  ifstream stop_words_file("res/yuliao/stop_words_zh.txt");
  if (!stop_words_file) {
    cerr << "Failed to open stop words file for filtering." << endl;
    return;
  }

  // 加载停用词到 unordered_set
  unordered_set<string> stop_words;
  string stop_word;
  while (stop_words_file >> stop_word) {
    stop_words.insert(stop_word);
  }

  string line;
  while (getline(cleaned_file, line)) {
    istringstream iss(line);
    string word;

    while (iss >> word) {
      // 过滤掉英文字符、标点符号，只保留中文字符
      std::string filtered_word;
      for (size_t i = 0; i < word.size();) {

        if (unsigned char c = word[i];c <= 0x7F) {  // 单字节字符 (ASCII)
          if (!is_english_char(c) && !is_punctuation(c)) {
            filtered_word += c;  // 保留非英文和非标点的 ASCII 字符
          }
          ++i;
        } else if ((c & 0xE0) == 0xC0) {  // 双字节字符
          if (i + 1 < word.size()) {
            if (uint32_t codepoint = ((c & 0x1F) << 6) | word[i + 1] & 0x3F;is_chinese_char(codepoint)) {
              filtered_word += word.substr(i, 2);  // 保留中文字符
            }
          }
          i += 2;
        } else if ((c & 0xF0) == 0xE0) {  // 三字节字符
          if (i + 2 < word.size()) {
            uint32_t codepoint = (c & 0x0F) << 12 |
                                 (word[i + 1] & 0x3F) << 6 |
                                 word[i + 2] & 0x3F;
            if (is_chinese_char(codepoint)) {
              filtered_word += word.substr(i, 3);  // 保留中文字符
            }
          }
          i += 3;
        } else {  // 非法字符或不支持的编码
          ++i;
        }
      }

      // 如果过滤后的词为空或者是停用词，则跳过
      if (filtered_word.empty() || stop_words.count(filtered_word)) {
        continue;
      }

      // 如果字典中已有该词，则增加词频；否则，添加新的词到词典
      auto it = find_if(
          dict_.begin(), dict_.end(),
          [&word](const pair<string, int> &p) { return p.first == word; });
      if (it != dict_.end()) {
        it->second++;  // 增加词频
      } else {
        dict_.emplace_back(word, 1);
      }
    }
  }

  // 对词典按词频降序、字母升序排序
  std::sort(dict_.begin(), dict_.end(),
            [](const pair<string, int> &a, const pair<string, int> &b) {
              if (a.second != b.second) {
                return a.second > b.second;  // 词频降序
              }
              return a.first < b.first;  // 词频相同时按字母升序
            });

  cout << "Chinese dictionary built with " << dict_.size() << " unique words."
       << endl;
}

void DictProducer::CreateIndex() {}

void DictProducer::Store() {
  // 导出英文词典
  const string output_path = "res/EnDict.dat";
  ofstream ofs(output_path, std::ios::out | std::ios::binary);

  if (!ofs) {
    cerr << "Failed to open output file: " << output_path << endl;
    return;
  }

  for (const auto &[fst, snd] : dict_) {
    ofs << fst << " " << snd
        << "\n";  // 格式化输出，使用 C++17 结构化绑定：单词 词频
  }

  cout << "Dictionary successfully saved to " << output_path << endl;

  // 清理临时文件
  const string temp_dir = "res/temp";

  try {
    for (const auto &entry : fs::directory_iterator(temp_dir)) {
      if (entry.is_regular_file()) {
        fs::remove(entry.path());
        cout << "Deleted temporary file: " << entry.path() << endl;
      }
    }
  } catch (const std::exception &e) {
    cerr << "Error cleaning temporary files: " << e.what() << endl;
  }
}

void DictProducer::ReadEnFile(const string &dir) {
  static const string temp_file_path = "res/temp/cleanedTextEn.txt";

  ifstream ifs(dir);
  if (!ifs) {
    cerr << "Failed to open file: " << dir << endl;
    return;
  }

  ofstream ofs(temp_file_path, std::ios::app);  // 追加模式打开临时文件
  if (!ofs) {
    cerr << "Failed to open temporary cleaned file: " << temp_file_path << endl;
    return;
  }

  string line;
  while (std::getline(ifs, line)) {
    if (string cleaned_line = CleanText(line); !cleaned_line.empty()) {
      ofs << cleaned_line << "\n";
    }
  }
  cout << "Cleaned content from file: " << dir << " appended to "
       << temp_file_path << endl;
}

void DictProducer::ReadCnFile(const string &dir) {
  static const string temp_file_path =
      "res/temp/cleanedTextCn.txt";  // 临时文件路径

  ifstream ifs(dir);
  if (!ifs) {
    cerr << "Failed to open file: " << dir << endl;
    return;
  }

  ofstream ofs(temp_file_path, std::ios::app);  // 追加模式写入
  if (!ofs) {
    cerr << "Failed to open temporary cleaned file: " << temp_file_path << endl;
    return;
  }

  string line;
  while (std::getline(ifs, line)) {
    // 去掉换行符
    line.erase(remove(line.begin(), line.end(), '\r'), line.end());
    line.erase(remove(line.begin(), line.end(), '\n'), line.end());

    if (!cuttor_) {
      cerr << "SplitTool is not initialized." << endl;
      return;
    }

    // 分词
    vector<string> words = cuttor_->Cut(line);

    // 写入分词结果到临时文件
    for (const auto &word : words) {
      ofs << word << " ";
    }
    ofs << "\n";  // 每行一个完整的分词结果
  }

  cout << "Processed and segmented content from file: " << dir
       << " appended to " << temp_file_path << endl;
}

string DictProducer::CleanText(const string &lines) {
  string lower_text;
  // 大写转小写
  std::transform(lines.begin(), lines.end(), std::back_inserter(lower_text),
                 [](unsigned char c) { return tolower(c); });

  // 去除所有非小写字母的字符
  const regex non_letters("[^a-z ]");
  string cleaned_text = std::regex_replace(lower_text, non_letters, " ");

  // 去除连续空格字符
  const regex multiple_spaces("\\s+");
  cleaned_text = regex_replace(cleaned_text, multiple_spaces, " ");

  // 去除字符串开头和结尾的空格
  const size_t start = cleaned_text.find_first_not_of(' ');
  const size_t end = cleaned_text.find_last_not_of(' ');
  if (start == string::npos || end == string::npos) {
    return "";  // 如果字符串全是空格，则返回空字符串
  }
  return cleaned_text.substr(start, end - start + 1);
}

[[maybe_unused]] void DictProducer::GetFileName(const string &path) {
  for (const auto &entry : fs::directory_iterator(path)) {
    if (entry.is_regular_file()) {
      files_.push_back(entry.path().string());
    }
  }
}

bool isChineseCharacter(char32_t ch) {
  // 中文字符的 Unicode 范围：4e00 - 9fa5（简体汉字）
  return (ch >= 0x4e00 && ch <= 0x9fa5);
}