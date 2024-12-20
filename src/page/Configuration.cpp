#include "page/Configuration.h"

#include <bitsc++.h>

Configuration::Configuration(const string &file_path) : file_path_(file_path) {
  ifstream ifs(file_path);
  if (!ifs.is_open()) {
    cerr << "Failed to open configuration file: " << file_path << endl;
    throw runtime_error("Configuration file not found.");
  }

  // 解析 JSON 文件
  json j;
  ifs >> j;
  ifs.close();

  // 分别解析配置文件的不同部分
  ParseCorpusDirs(j);
  ParseStopWordsPaths(j);
  ParsePagesPath(j);
}

map<string, string> &Configuration::GetConfigMap() { return config_map_; }

set<string> &Configuration::GetStopWordsCn() {
  if (!stop_words_cn_loaded_) {
    if (const auto it_cn = config_map_.find("stop_words_cn_path");
        it_cn != config_map_.end()) {
      LoadStopWords(it_cn->second, stop_words_cn_);
      stop_words_cn_loaded_ = true;
    } else {
      throw std::runtime_error(
          "Missing Chinese stop words path in configuration.");
    }
  }
  return stop_words_cn_;
}

set<string> &Configuration::GetStopWordsEn() {
  if (!stop_words_en_loaded_) {
    if (const auto it_en = config_map_.find("stop_words_en_path");
        it_en != config_map_.end()) {
      LoadStopWords(it_en->second, stop_words_en_);
      stop_words_en_loaded_ = true;
    } else {
      throw std::runtime_error(
          "Missing English stop words path in configuration.");
    }
  }
  return stop_words_en_;
}

string Configuration::GetPagesPath() const {
  if (const auto it = config_map_.find("pages");it != config_map_.end()) {
    return it->second;
  }
  throw runtime_error("Missing 'pages' path in configuration.");
}

void Configuration::ParseCorpusDirs(const json &j) {
  if (j.contains("corpus_dirs")) {
    auto corpus_dirs = j["corpus_dirs"];
    if (corpus_dirs.contains("english")) {
      config_map_["corpus_dir_en"] = corpus_dirs["english"].get<std::string>();
    }
    if (corpus_dirs.contains("chinese")) {
      config_map_["corpus_dir_cn"] = corpus_dirs["chinese"].get<std::string>();
    }
  }
}

void Configuration::ParseStopWordsPaths(const json &j) {
  if (j.contains("stop_words_path")) {
    auto stop_words_path = j["stop_words_path"];
    if (stop_words_path.contains("english")) {
      config_map_["stop_words_en_path"] = stop_words_path["english"].get<std::string>();
    }
    if (stop_words_path.contains("chinese")) {
      config_map_["stop_words_cn_path"] = stop_words_path["chinese"].get<std::string>();
    }
  }
}

void Configuration::ParsePagesPath(const json &j) {
  if (j.contains("pages")) {
    config_map_["pages"] = j["pages"].get<std::string>();
  }
}

void Configuration::LoadStopWords(const string &path, set<string> &stop_words) {
  ifstream ifs(path);
  if (!ifs.is_open()) {
    cerr << "Failed to open stop words file: " << path << endl;
    throw runtime_error("Stop words file not found.");
  }

  string word;
  while (ifs >> word) {
    stop_words.insert(word);
  }

  ifs.close();
  cout << "Loaded " << stop_words.size() << " stop words from " << path << endl;
}