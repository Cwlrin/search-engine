#include "page/DirScanner.h"

DirScanner::DirScanner(Configuration& config) : config_(config){}

// 重载函数调用运算符，调用 Traverse 函数
void DirScanner::operator()() {
  // 获取配置中的目录路径
  const auto &config_map = config_.GetConfigMap();
  for (const auto & [fst, snd] : config_map) {
    if (fst.find("corpus_dir") != string::npos) {
      Traverse(snd);
    }
  }
}

// 递归遍历目录
void DirScanner::Traverse(const string& dir_name) {
  try {
    for (const auto &entry : fs::recursive_directory_iterator(dir_name)) {
      if (entry.is_regular_file()) {
        files_.push_back(entry.path().string());
      }
    }
  } catch (const std::exception &e) {
    cerr << "Error while traversing directory " << dir_name << ": " << e.what() << endl;
  }
}

vector<string> DirScanner::Files() const {
  return files_;
}

