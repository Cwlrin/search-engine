//
// Created by 翠 on 24-11-26.
//

#ifndef WEBPAGE_H
#define WEBPAGE_H
#include "dictionary/SplitTool.h"

class WebPage {
 public:
  WebPage(string &doc, Configuration &conf,
          SplitTool *jieba);  // 获取文档的 doc_id_
  [[nodiscard]] int GetDocId() const;
  // 获取文档
  [[nodiscard]] string GetDoc() const;
  // 获取文档的词频统计 map
  [[nodiscard]] map<string, int> GetWordsMap() const;

 private:
  // 整篇文档，包括 xml 在内
  string doc_;
  // 文档 id
  int doc_id_ = 0;
  // 文档标题
  string doc_title_;
  // 文档 URL
  string doc_url_;
  // 文档内容
  string doc_content_;
  // 保存每篇文档的所有词语和词频，不包括停用词
  map<string, int> words_map_;
  // 对格式化文档进行处理
  void ProcessDoc(const string &doc, Configuration &config, SplitTool &);
  // 求出文档的 topk 词集
  void CalcTopK(vector<string> &words_vec, int k, set<string> &stop_word_list);
  // 判断两篇文档是否相同
  friend bool operator==(const WebPage &, const WebPage &);
  // 对文档按照 doc_id_ 进行排序
  friend bool operator<(const WebPage &, const WebPage &);
};

#endif
