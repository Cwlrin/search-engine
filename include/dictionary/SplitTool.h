#ifndef _SPLIT_TOOL_H_
#define _SPLIT_TOOL_H_

#include "Dictionary.h"

using std::string;
using std::vector;

class SplitTool {
 public:
  virtual vector<string> cut(const string& dir);
};

#endif  // _SPLIT_TOOL_H_