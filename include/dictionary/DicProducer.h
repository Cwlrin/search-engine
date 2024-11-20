#ifndef _DICT_PRODUCER_H_
#define _DICT_PRODUCER_H_

#include "Dictionary.h"
#include "SplitTool.h"

using std::string;

class DictProducer {
 public:
  // 构造函数
  explicit DictProducer(const string &dir);

  // 构造函数,专为中文处理
  DictProducer(const string &dir, SplitTool *splitTool);

  // 创建英文字典
  void buildEnDict();

  // 创建中文字典
  void buildCnDict();

  // 生成位置索引
  void createIndex();

  // 将词典写入文件
  void store();

  // 返回文件路径列表的副本
  [[nodiscard]] const vector<string> &getFiles() const {
    return _files;
  }

  // 文件读取并清洗
  static void readFile(const string &dir);

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
  // 停用词列表
  vector<string> _stopWords;

 private:

  // 按行清洗语料库
  static string cleanText(const string &lines);

  // 获取文件名
  [[maybe_unused]] void getFileName(const string &path);
};

#endif  // _DICT_PRODUCER_H_
