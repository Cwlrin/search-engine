#ifndef DIRSCANNER_H
#define DIRSCANNER_H

#include "bitsc++.h"

class DirScanner {
 public:
  explicit DirScanner(string base_dir);
  // 执行目录扫描
  void Traverse();
  // 获取扫描到的文件路径列表
  [[nodiscard]] const vector<string> &GetFiles() const;
 private:
  // 基础目录
  string base_dir_;
  // 存储文件路径
  vector<string> files_;
  // 递归扫描目录
  void TraverseDirectory(const string &dir);
};

#endif
