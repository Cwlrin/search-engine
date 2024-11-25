#ifndef _SPLIT_TOOL_H_
#define _SPLIT_TOOL_H_

#include "bitsc++.h"

class SplitTool {
 public:
  virtual ~SplitTool() = default;
  virtual vector<string> Cut(const string &) = 0;
};

#endif