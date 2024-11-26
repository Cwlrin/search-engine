#ifndef CONFIGURATION_H_
#define CONFIGURATION_H_

#include "bitsc++.h"

class Configuration {
 public:
  explicit Configuration(const string &file_path);
  // 获取存放配置文件内容的 map
  map<string, string> &GetConfigMap();
  // 获取停用词词集
  set<string> &GetStopWordList();

 private:
  // 配置文件路径
  string file_path_;
  // 配置文件内容
  map<string, string> config_map_;
  // 停用词词集
  set<string> stop_words_;
};

#endif