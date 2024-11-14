#ifndef _SPLIT_TOOL_CPP_JIEBA_H_
#define __SPLIT_TOOL_CPP_JIEBA_H__

#include "Dictionary.h"
#include "SplitTool.h"

class SplitToolCppJieba : public SplitTool {
 public:
  SplitToolCppJieba();
  vector<string> cut(const string &dir) override;
};
#endif