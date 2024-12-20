#ifndef DIRSCANNER_H
#define DIRSCANNER_H

#include "Configuration.h"
#include "bitsc++.h"

class DirScanner {
 public:
  explicit DirScanner(Configuration &config);
  // 重载函数调用运算符，调用 traverse 函数
  void operator()();
  // 返回文件列表
  [[nodiscard]] vector<string> Files() const;
 private:
  // 配置文件引用
  Configuration &config_;
  // 存储文件路径
  vector<string> files_;
  // 获取某一目录下的所有文件
  void Traverse(const string &dir_name);
};

#endif
