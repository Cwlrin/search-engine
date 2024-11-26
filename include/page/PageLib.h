#ifndef PAGELIB_H
#define PAGELIB_H

#include "Configuration.h"
#include "DirScanner.h"
#include "FileProcessor.h"
#include "bitsc++.h"

class PageLib {
 public:
  PageLib(Configuration &conf, DirScanner &dir_scanner,
          FileProcessor &file_processor);
  // 创建网页库
  void Create();
  // 存储网页库和位置偏移库
  void Store();
 private:
  // 目录扫描对象的引用
  DirScanner &dir_scanner_;
  // 存放格式化之后的网页的容器
  vector<string> pages_;
  // 存放每篇文档在网页库的位置信息
  map<int, pair<int, int>> offset_lab_;
};



#endif
