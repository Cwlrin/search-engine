#ifndef PAGELIBPREPROCESSER_H
#define PAGELIBPREPROCESSER_H

#include "Configuration.h"
#include "DirScanner.h"
#include "WebPage.h"
#include "bitsc++.h"
#include "dictionary/SplitTool.h"

class PageLibPreprocesser {
 public:
  explicit PageLibPreprocesser(Configuration *config);
  // 执行预处理
  void DoProcess();

 private:
  // 分词对象
  SplitTool *word_cutter_ = nullptr;
  // 网页偏移库对象
  unordered_map<int, pair<int, int>> offset_lib_;
  // 倒排索引表对象
  unordered_map<string, vector<pair<int, double>>> invert_index_table_;
  // 根据配置信息读取网页库和位置偏移库的内容
  void ReadInfoFromFile();
  // 对冗余的网页进行去重
  void CutRedundantPages();
  // 创建倒排索引表
  void BuildInvertIndexTable();
  // 将经过预处理之后的网页库、位置偏移库和倒排索引表写回到磁盘上
  void StoreOnDisk();
};

#endif  // PAGELIBPREPROCESSER_H
