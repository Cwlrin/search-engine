#include "page/DirScanner.h"

DirScanner::DirScanner(string base_dir) : base_dir_(std::move(base_dir)) {
  if (!fs::exists(base_dir_) || !fs::is_directory(base_dir_)) {
    throw std::runtime_error("Invalid base directory: " + base_dir_);
  }
}

void DirScanner::Traverse() {
  files_.clear();  // 清空之前的扫描结果
  TraverseDirectory(base_dir_);
}

const vector<string> &DirScanner::GetFiles() const {
  return files_;
}

void DirScanner::TraverseDirectory(const string &dir) {
  for (const auto &entry : fs::directory_iterator(dir)) {
    if (entry.is_directory()) {
      // 如果是子目录，则递归扫描
      TraverseDirectory(entry.path().string());
    } else if (entry.is_regular_file()) {
      // 如果是文件，则记录文件路径
      files_.emplace_back(entry.path().string());
    }
  }
}