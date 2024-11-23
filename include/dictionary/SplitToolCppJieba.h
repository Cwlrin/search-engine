#ifndef _SPLIT_TOOL_CPP_JIEBA_H_
#define _SPLIT_TOOL_CPP_JIEBA_H_

#include "SplitTool.h"
#include "bitsc++.h"
#include "cppjieba/Jieba.hpp"

class SplitToolCppJieba : public SplitTool {
 public:
  vector<string> Cut(const string &) override;

 private:
  static cppjieba::Jieba cuttor_;
};

#endif