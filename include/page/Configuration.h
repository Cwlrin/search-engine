#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "bitsc++.h"

class Configuration {
 public:
  explicit Configuration(const string &file_path);

  // 获取存放配置文件内容的 map
  map<string, string> &GetConfigMap();
  // 获取中文停用词词集
  set<string> &GetStopWordsCn();
  // 获取英文停用词词集
  set<string> &GetStopWordsEn();

 private:
  // 配置文件路径
  string file_path_;
  // 配置文件内容
  map<string, string> config_map_;
  // 中文停用词词集
  set<string> stop_words_cn_;
  // 英文停用词词集
  set<string> stop_words_en_;

  // 从 JSON 文件解析配置
  void ParseConfigFromJson(const json &j);
  // 从路径加载停用词
  void LoadStopWords(const string &path, set<string> &stop_words);
};

#endif