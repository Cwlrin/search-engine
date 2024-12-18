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

  // 从 JSON 解析配置内容
  ParseConfigFromJson(j);

  // 加载中文停用词
  auto it_cn = config_map_.find("stop_words_cn_path");
  if (it_cn != config_map_.end()) {
    LoadStopWords(it_cn->second, stop_words_cn_);
  }

  // 加载英文停用词
  auto it_en = config_map_.find("stop_words_en_path");
  if (it_en != config_map_.end()) {
    LoadStopWords(it_en->second, stop_words_en_);
  }
}

map<string, string> &Configuration::GetConfigMap() {
  return config_map_;
}

set<string> &Configuration::GetStopWordsCn() {
  return stop_words_cn_;
}

set<string> &Configuration::GetStopWordsEn() {
  return stop_words_en_;
}

void Configuration::ParseConfigFromJson(const json &j) {
  // 解析 corpus_dirs
  if (j.contains("corpus_dirs")) {
    auto corpus_dirs = j["corpus_dirs"];
    if (corpus_dirs.contains("english")) {
      config_map_["corpus_dir_en"] = corpus_dirs["english"].get<string>();
    }
    if (corpus_dirs.contains("chinese")) {
      config_map_["corpus_dir_cn"] = corpus_dirs["chinese"].get<string>();
    }
  }

  // 解析 stop_words_path
  if (j.contains("stop_words_path")) {
    auto stop_words_path = j["stop_words_path"];
    if (stop_words_path.contains("english")) {
      config_map_["stop_words_en_path"] = stop_words_path["english"].get<string>();
    }
    if (stop_words_path.contains("chinese")) {
      config_map_["stop_words_cn_path"] = stop_words_path["chinese"].get<string>();
    }
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