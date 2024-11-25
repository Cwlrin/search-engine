#include "dictionary/DicProducer.h"

// 判断单词类型的辅助函数
enum class WordType { CHINESE_WORD, ENGLISH_WORD, OTHER };

DictProducer::DictProducer(const string &dir) {
  ifstream config_file(dir);
  if (!config_file.is_open()) {
    throw runtime_error("Failed to open configuration file.");
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
    throw runtime_error(
        "SplitTool is required for Chinese dictionary generation.");
  }

  ifstream config_file(dir);
  if (!config_file.is_open()) {
    throw runtime_error("Failed to open configuration file.");
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

  // 将停用词加载到哈希集合 unordered_set 中供快速查找
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
      if (stop_words.count(word)) {
        continue;  // 忽略停用词
      }

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
  sort(dict_.begin(), dict_.end(),
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
  const string punctuations =
      ".,!?;:\"'()[]{}<>~`@#$%^&*-+=_|\\/，。！？；：“”‘’（）【】《》……";
  return punctuations.find(c) != string::npos;
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
      string filtered_word;
      for (size_t i = 0; i < word.size();) {
        if (unsigned char c = word[i]; c <= 0x7F) {  // 单字节字符 (ASCII)
          if (!is_english_char(c) && !is_punctuation(c)) {
            filtered_word += c;  // 保留非英文和非标点的 ASCII 字符
          }
          ++i;
        } else if ((c & 0xE0) == 0xC0) {  // 双字节字符
          if (i + 1 < word.size()) {
            if (uint32_t codepoint = (c & 0x1F) << 6 | word[i + 1] & 0x3F;
                is_chinese_char(codepoint)) {
              filtered_word += word.substr(i, 2);  // 保留中文字符
            }
          }
          i += 2;
        } else if ((c & 0xF0) == 0xE0) {  // 三字节字符
          if (i + 2 < word.size()) {
            uint32_t codepoint = (c & 0x0F) << 12 | (word[i + 1] & 0x3F) << 6 |
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
  sort(dict_.begin(), dict_.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         if (a.second != b.second) {
           return a.second > b.second;  // 词频降序
         }
         return a.first < b.first;  // 词频相同时按字母升序
       });

  cout << "Chinese dictionary built with " << dict_.size() << " unique words."
       << endl;
}

void DictProducer::CreateIndex() {
  indexs_.clear();  // 清空旧的索引

  // 遍历词典中的每个词
  for (size_t i = 0; i < dict_.size(); ++i) {
    const string &word = dict_[i].first;

    // 遍历词的每个字符（支持中文和英文）
    for (size_t j = 0; j < word.size();) {
      const unsigned char c = word[j];
      string character;

      if (c <= 0x7F) {  // 单字节字符 (ASCII)，可能是英文
        character = word.substr(j, 1);
        indexs_[character].insert(
            static_cast<int>(i));  // 将该字母映射到词的索引
        ++j;
      } else if ((c & 0xE0) == 0xC0) {  // 双字节字符
        character = word.substr(j, 2);
        indexs_[character].insert(static_cast<int>(i));
        j += 2;
      } else if ((c & 0xF0) == 0xE0) {  // 三字节字符 (中文)
        character = word.substr(j, 3);
        indexs_[character].insert(static_cast<int>(i));
        j += 3;
      } else {
        cerr << "Invalid UTF-8 character encountered in word: " << word << endl;
        ++j;
      }
    }
  }

  cout << "Index created with " << indexs_.size() << " unique characters."
       << endl;
}

WordType check_word_type(const string &word) {
  bool is_chinese = true;
  bool is_english = true;

  for (size_t i = 0; i < word.size();) {
    if (const unsigned char c = word[i]; c <= 0x7F) {  // 单字节字符 (ASCII)
      if (!isalpha(c)) {
        is_english = false;  // 非英文字母
      }
      is_chinese = false;  // ASCII 字符不可能是中文
      ++i;
    } else if ((c & 0xF0) == 0xE0) {  // 三字节字符 (UTF-8)
      if (i + 2 < word.size()) {
        const uint32_t codepoint =
            (c & 0x0F) << 12 | (word[i + 1] & 0x3F) << 6 | word[i + 2] & 0x3F;
        if (codepoint < 0x4E00 || codepoint > 0x9FA5) {
          is_chinese = false;  // 非中文字符
        }
      }
      is_english = false;  // 三字节字符不可能是英文
      i += 3;
    } else {  // 其他字符
      is_chinese = false;
      is_english = false;
      ++i;
    }
  }

  if (is_chinese) {
    return WordType::CHINESE_WORD;
  }
  if (is_english) {
    return WordType::ENGLISH_WORD;
  }
  return WordType::OTHER;
}

void DictProducer::StoreDict() {
  const string cn_output_path = "res/CnDict.dat";
  const string en_output_path = "res/EnDict.dat";

  // 打开文件（仅当需要写入时）
  ofstream cn_ofs, en_ofs;

  if (!cn_ofs || !en_ofs) {
    cerr << "Failed to open output files for saving the dictionaries." << endl;
    return;
  }

  for (const auto &[fst, snd] : dict_) {
    const string &word = fst;
    int frequency = snd;

    if (WordType type = check_word_type(word); type == WordType::CHINESE_WORD) {
      if (!cn_ofs.is_open()) {
        cn_ofs.open(cn_output_path, ios::out | ios::binary);
        if (!cn_ofs) {
          cerr << "Failed to open Chinese dictionary file: " << cn_output_path
               << endl;
          return;
        }
      }
      cn_ofs << word << " " << frequency << "\n";  // 保存到中文词典
    } else if (type == WordType::ENGLISH_WORD) {
      if (!en_ofs.is_open()) {
        en_ofs.open(en_output_path, ios::out | ios::binary);
        if (!en_ofs) {
          cerr << "Failed to open English dictionary file: " << en_output_path
               << endl;
          return;
        }
      }
      en_ofs << word << " " << frequency << "\n";  // 保存到英文词典
    }
  }
  cout << "Dictionaries successfully saved to " << cn_output_path << " and "
       << en_output_path << "." << endl;

  if (cn_ofs.is_open()) {
    cn_ofs.close();
    cout << "Chinese dictionary successfully saved to " << cn_output_path << "."
         << endl;
  }
  if (en_ofs.is_open()) {
    en_ofs.close();
    cout << "English dictionary successfully saved to " << en_output_path << "."
         << endl;
  }

  // 清理临时文件
  const string temp_dir = "res/temp";

  try {
    for (const auto &entry : fs::directory_iterator(temp_dir)) {
      if (entry.is_regular_file()) {
        fs::remove(entry.path());
        cout << "Deleted temporary file: " << entry.path() << endl;
      }
    }
  } catch (const exception &e) {
    cerr << "Error cleaning temporary files: " << e.what() << endl;
  }
}

void DictProducer::StoreIndex(const map<string, set<int>> &combined_index) {
  const string index_output_path = "res/Index.dat";

  // 打开文件（覆盖模式，每次重新写入整个索引）
  ofstream ofs(index_output_path, ios::out | ios::binary);

  if (!ofs) {
    cerr << "Failed to open index file for saving: " << index_output_path
         << endl;
    return;
  }

  // 遍历索引并写入文件
  for (const auto &[character, word_set] : combined_index) {
    ofs << character << " ";  // 写入字符
    for (const int idx : word_set) {
      ofs << idx << " ";  // 写入词典索引
    }
    ofs << "\n";  // 换行分隔不同的字符
  }

  cout << "Index successfully saved to " << index_output_path << "." << endl;
}

void DictProducer::ReadEnFile(const string &dir) {
  static const string temp_file_path = "res/temp/cleanedTextEn.txt";

  ifstream ifs(dir);
  if (!ifs) {
    cerr << "Failed to open file: " << dir << endl;
    return;
  }

  ofstream ofs(temp_file_path, ios::app);  // 追加模式打开临时文件
  if (!ofs) {
    cerr << "Failed to open temporary cleaned file: " << temp_file_path << endl;
    return;
  }

  string line;
  while (getline(ifs, line)) {
    if (string cleaned_line = CleanText(line); !cleaned_line.empty()) {
      ofs << cleaned_line << "\n";
    }
  }
  cout << "Cleaned content from file: " << dir << " appended to "
       << temp_file_path << endl;
}

void DictProducer::ReadCnFile(const string &dir) const {
  static const string temp_file_path =
      "res/temp/cleanedTextCn.txt";  // 临时文件路径

  ifstream ifs(dir);
  if (!ifs) {
    cerr << "Failed to open file: " << dir << endl;
    return;
  }

  ofstream ofs(temp_file_path, ios::app);  // 追加模式写入
  if (!ofs) {
    cerr << "Failed to open temporary cleaned file: " << temp_file_path << endl;
    return;
  }

  string line;
  while (getline(ifs, line)) {
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
  transform(lines.begin(), lines.end(), back_inserter(lower_text),
            [](const unsigned char c) { return tolower(c); });

  // 去除所有非小写字母的字符
  const regex non_letters("[^a-z ]");
  string cleaned_text = regex_replace(lower_text, non_letters, " ");

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

void DictProducer::LoadDict(const string &path) {
  ifstream ifs(path, ios::in);
  if (!ifs) {
    cerr << "Failed to open dictionary file: " << path << endl;
    return;
  }

  dict_.clear();  // 清空当前词典内容

  string word;
  int frequency;
  while (ifs >> word >> frequency) {
    dict_.emplace_back(word, frequency);  // 重新加载词典到内存
  }

  cout << "Loaded dictionary from " << path << " with " << dict_.size() << " entries." << endl;
}