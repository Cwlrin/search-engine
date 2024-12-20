#include "dictionary/DicProducer.h"

DictProducer::DictProducer(Configuration &config) : config_(config) {
  // 获取英文语料路径
  auto &config_map = config.GetConfigMap();
  if (const auto it = config_map.find("corpus_dir_en");
      it != config_map.end()) {
    const string english_path = it->second;
    for (const auto &entry : fs::directory_iterator(english_path)) {
      if (entry.is_regular_file()) {
        files_en_.push_back(entry.path().string());
      }
    }
  }

  // 加载英文停用词
  stop_words_ = config.GetStopWordsEn();
}

DictProducer::DictProducer(Configuration &config, SplitTool *split_tool)
    : split_tool_(split_tool), config_(config) {
  if (!split_tool_) {
    throw runtime_error(
        "SplitTool is required for Chinese dictionary generation.");
  }

  // 获取中文语料路径
  auto &config_map = config.GetConfigMap();
  if (const auto it = config_map.find("corpus_dir_cn");
      it != config_map.end()) {
    const string chinese_path = it->second;
    for (const auto &entry : fs::directory_iterator(chinese_path)) {
      if (entry.is_regular_file()) {
        files_cn_.push_back(entry.path().string());
      }
    }
  }

  // 加载中文停用词
  stop_words_ = config.GetStopWordsCn();
}

void DictProducer::ReadFile(const string &file_path) {
  ifstream ifs(file_path);
  if (!ifs.is_open()) {
    cerr << "Failed to open file: " << file_path << endl;
    return;
  }
  string line;
  while (getline(ifs, line)) {
    // 英文清洗和词频统计（仅匹配由小写字母组成的单词）
    transform(line.begin(), line.end(), line.begin(), tolower);
    regex word_regex(R"(\b[a-z]+\b)");
    auto words_begin = sregex_iterator(line.begin(), line.end(), word_regex);
    auto words_end = sregex_iterator();
    for (auto it = words_begin; it != words_end; ++it) {
      if (string word = it->str(); !stop_words_.count(word)) {
        auto dict_it = find_if(
            dict_.begin(), dict_.end(),
            [&word](const pair<string, int> &p) { return p.first == word; });
        if (dict_it != dict_.end()) {
          dict_it->second++;
        } else {
          dict_.emplace_back(word, 1);
        }
      }
    }
  }
}

void DictProducer::BuildEnDict() {
  dict_.clear();  // 清空词典
  for (const auto &file : files_en_) {
    ReadFile(file);
  }
  cout << "English dictionary built with " << dict_.size() << " unique words."
       << endl;
}

bool is_valid_chinese(const string &word) {
  for (size_t i = 0; i < word.size();) {
    if (const unsigned char c = word[i]; (c & 0xF0) == 0xE0) {  // UTF-8 三字节
      if (i + 2 < word.size()) {
        const uint32_t codepoint =
            (c & 0x0F) << 12 | (word[i + 1] & 0x3F) << 6 | (word[i + 2] & 0x3F);
        if (codepoint < 0x4E00 || codepoint > 0x9FA5) {
          return false;  // 非中文字符
        }
      }
      i += 3;
    } else {
      return false;  // 非合法 UTF-8 中文字符
    }
  }
  return true;
}

void DictProducer::BuildCnDict() {
  dict_.clear();  // 清空词典
  for (const auto &file : files_cn_) {
    ifstream ifs(file);
    if (!ifs.is_open()) {
      cerr << "Failed to open file: " << file << endl;
      continue;
    }
    string line;
    while (getline(ifs, line)) {
      vector<string> words = split_tool_->Cut(line);
      for (const auto &word : words) {
        if (is_valid_chinese(word) &&
            !stop_words_.count(word)) {  // 仅保留中文字符
          auto it = find_if(
              dict_.begin(), dict_.end(),
              [&word](const pair<string, int> &p) { return p.first == word; });
          if (it != dict_.end()) {
            it->second++;
          } else {
            dict_.emplace_back(word, 1);
          }
        }
      }
    }
  }

  cout << "Chinese dictionary built with " << dict_.size() << " unique words."
       << endl;
}

void DictProducer::CreateIndex() {
  indexs_.clear();
  for (size_t i = 0; i < dict_.size(); ++i) {
    const string &word = dict_[i].first;
    for (size_t j = 0; j < word.size();) {
      string character;
      if (const unsigned char c = word[j]; c <= 0x7F) {
        character = word.substr(j, 1);  // 英文
        ++j;
      } else if ((c & 0xF0) == 0xE0) {
        character = word.substr(j, 3);  // 中文
        j += 3;
      } else {
        ++j;
      }
      indexs_[character].insert(static_cast<int>(i));
    }
  }
}

void DictProducer::StoreDict() {
  StoreCnDict();
  StoreEnDict();
}

void DictProducer::StoreCnDict() {
  const string cn_output_path = "res/CnDict.dat";
  ofstream cn_ofs(cn_output_path, ios::out | ios::binary);

  if (!cn_ofs) {
    cerr << "Failed to open Chinese dictionary file: " << cn_output_path
         << endl;
    return;
  }
  // 按词频降序排列，同词频按字典序升序排列
  sort(dict_.begin(), dict_.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         if (a.second != b.second) {
           return a.second > b.second;  // 词频降序
         }
         return a.first < b.first;  // 词频相同时按字典序升序
       });

  for (const auto &[word, frequency] : dict_) {
    cn_ofs << word << " " << frequency << "\n";  // 保存中文词典
  }
  cout << "Chinese dictionary successfully saved to " << cn_output_path << "."
       << endl;
}

void DictProducer::StoreEnDict() {
  const string en_output_path = "res/EnDict.dat";
  ofstream en_ofs(en_output_path, ios::out | ios::binary);

  if (!en_ofs) {
    cerr << "Failed to open English dictionary file: " << en_output_path
         << endl;
    return;
  }
  // 按词频降序排列，同词频按字典序升序排列
  sort(dict_.begin(), dict_.end(),
       [](const pair<string, int> &a, const pair<string, int> &b) {
         if (a.second != b.second) {
           return a.second > b.second;  // 词频降序
         }
         return a.first < b.first;  // 词频相同时按字典序升序
       });

  for (const auto &[word, frequency] : dict_) {
    en_ofs << word << " " << frequency << "\n";  // 保存英文词典
  }
  cout << "English dictionary successfully saved to " << en_output_path << "."
       << endl;
}

void DictProducer::StoreIndex() {
  const string index_output_path = "res/Index.dat";

  ofstream ofs(index_output_path, ios::app | ios::binary);

  if (!ofs.is_open()) {
    cerr << "Failed to open index file for saving: " << index_output_path
         << endl;
    return;
  }

  // 遍历索引并写入文件
  for (const auto &[character, word_set] : indexs_) {
    ofs << character << " ";  // 写入字符
    for (const int idx : word_set) {
      ofs << idx << " ";  // 写入词典索引
    }
    ofs << "\n";  // 换行分隔不同的字符
  }

  cout << "Index successfully saved to " << index_output_path << "." << endl;
}

string DictProducer::CleanText(const string &lines) {
  string lower_text;
  transform(lines.begin(), lines.end(), back_inserter(lower_text), ::tolower);
  return regex_replace(lower_text, regex("[^a-z ]"), " ");
}