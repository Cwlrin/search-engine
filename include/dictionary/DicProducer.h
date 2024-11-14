#ifndef _DICT_PRODUCER_H_
#define _DICT_PRODUCER_H_

#include "Dictionary.h"
#include "SplitTool.h"

using std::string;

class DictProducer {
 public:
  DictProducer();
  // 构造函数
  DictProducer(const string &dir);
  // 构造函数,专为中文处理
  DictProducer(const string &dir, SplitTool *splitTool);
  // 创建英文字典
  void buildEnDict();
  // 创建中文字典
  void bulidCnDict();
  // 生成位置索引
  void createIndex();
  // 将词典写入文件
  void store();

 private:
  // 语料库文件的绝对路径集合
  vector<string> _files;
  // 词典
  vector<pair<string, int>> _dict;
  // 词典索引
  map<string, set<int>> _indexs;
  // 分词工具
  SplitTool _cuttor;
  // 临时缓存
  string _cleanedLine;

 private:
  // 文件读取并清洗
  void readFile(const string &dir);
  // 按行清洗语料库
  string cleanText(const string &lines);
};
#endif  // _DICT_PRODUCER_H_
