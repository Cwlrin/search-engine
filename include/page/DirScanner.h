#ifndef DIRSCANNER_H
#define DIRSCANNER_H

#include "bitsc++.h"

class DirScanner {
 public:
  DirScanner();
  // 重载函数调用运算符，调用 traverse 函数
  void operator()();
  // 返回文件列表
  vector<string> Files();
  // 获取某一目录下的所有文件
  void Traverse(const string &dir_name);

 private:
  vector<string> files_;
};

#endif
