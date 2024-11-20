#ifndef _SPLIT_TOOL_CPP_JIEBA_H_
#define _SPLIT_TOOL_CPP_JIEBA_H_

#include "Dictionary.h"
#include "SplitTool.h"

class SplitToolCppJieba : public SplitTool {
 public:
  SplitToolCppJieba();
  vector<string> cut(const string &dir) override;
};
#endif