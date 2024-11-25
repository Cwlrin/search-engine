#ifndef _DICT_PRODUCER_H_
#define _DICT_PRODUCER_H_

#include "SplitTool.h"
#include "bitsc++.h"

class DictProducer {
 public:
  // 构造函数
  explicit DictProducer(const string &dir);

  // 构造函数,专为中文处理
  DictProducer(const string &dir, SplitTool *split_tool);

  // 创建英文字典
  void BuildEnDict();

  // 创建中文字典
  void BuildCnDict();

  // 生成位置索引
  void CreateIndex();

  // 将词典写入文件
  void StoreDict();

  // 将索引写入文件
  static void StoreIndex(const map<string, set<int>> &combined_index);

  // 返回文件路径列表的副本
  const vector<string> &GetFiles() const { return files_; }

  // 返回词典索引
  const map<string, set<int>> &GetIndex() const { return indexs_; }

  // 读取英文文件并清洗
  static void ReadEnFile(const string &dir);

  // 读取中文文件并分词和清洗
  void ReadCnFile(const string &dir) const;

  // 从导出的词典文件加载词典内容
  void LoadDict(const string &path);

 private:
  // 语料库文件的绝对路径集合
  vector<string> files_;
  // 词典
  vector<pair<string, int>> dict_;
  // 词典索引
  map<string, set<int>> indexs_;
  // 分词工具
  SplitTool *cuttor_{};
  // 临时缓存
  string cleaned_line_;
  // 停用词列表
  vector<string> stop_words_;

  // 按行清洗语料库
  static string CleanText(const string &lines);
};

#endif  // _DICT_PRODUCER_H_
