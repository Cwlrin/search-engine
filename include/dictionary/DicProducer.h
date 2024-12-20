#ifndef DICT_PRODUCER_H
#define DICT_PRODUCER_H

#include "SplitTool.h"
#include "bitsc++.h"
#include "page/Configuration.h"

class DictProducer {
 public:
  // 构造函数
  explicit DictProducer(Configuration &config);

  // 构造函数,专为中文处理
  DictProducer(Configuration &config, SplitTool *split_tool);

  // 创建英文字典
  void BuildEnDict();

  // 创建中文字典
  void BuildCnDict();

  // 生成位置索引
  void CreateIndex();

  // 将词典写入文件
  void StoreDict();

  // 将中文词典写入文件
  void StoreCnDict();

  // 将英文词典写入文件
  void StoreEnDict();

  // 将索引写入文件
  void StoreIndex();

 private:
  // 英文语料文件路径列表
  vector<string> files_en_;
  // 中文语料文件路径列表
  vector<string> files_cn_;
  // 词典
  vector<pair<string, int>> dict_;
  // 词典索引
  map<string, set<int>> indexs_;
  // 分词工具
  SplitTool *split_tool_ = nullptr;
  // 停用词列表
  set<string> stop_words_;
  // 配置引用
  Configuration &config_;

  // 从文件读取并清洗内容
  void ReadFile(const string &file_path);

  // 按行清洗语料库
  static string CleanText(const string &lines);
};

#endif  // DICT_PRODUCER_H
