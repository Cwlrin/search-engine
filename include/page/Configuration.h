#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "bitsc++.h"

class Configuration {
 public:
  explicit Configuration(const string &file_path);
  // 获取存放配置文件内容的 map
  map<string, string> &GetConfigMap();
  // 获取中文停用词词集（惰性加载）
  set<string> &GetStopWordsCn();
  // 获取英文停用词词集（惰性加载）
  set<string> &GetStopWordsEn();
  // 获取默认页面路径
  [[nodiscard]] string GetPagesPath() const;

 private:
  // 配置文件路径
  string file_path_;
  // 配置文件内容
  map<string, string> config_map_;
  // 中文停用词词集
  set<string> stop_words_cn_;
  // 英文停用词词集
  set<string> stop_words_en_;
  // 从 JSON 文件解析语料库路径
  void ParseCorpusDirs(const json &j);
  // 从 JSON 文件解析停用词路径
  void ParseStopWordsPaths(const json &j);
  // 从 JSON 文件解析默认页面路径
  void ParsePagesPath(const json &j);
  // 从 JSON 文件解析配置
  void ParseConfigFromJson(const json &j);
  // 从路径加载停用词
  static void LoadStopWords(const string &path, set<string> &stop_words);

  // 标记是否已加载中文停用词
  bool stop_words_cn_loaded_ = false;
  // 标记是否已加载英文停用词
  bool stop_words_en_loaded_ = false;
};

#endif