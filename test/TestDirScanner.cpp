#include <iostream>

#include "page/Configuration.h"
#include "page/DirScanner.h"

int main() {
  try {
    // 更改当前目录到项目根目录
    fs::current_path("/home/ichika/SearchEngine");

    // 指定扫描目录
    std::string base_dir = "res/page";

    // 创建 DirScanner 对象
    DirScanner dir_scanner(base_dir);

    // 扫描目录
    dir_scanner.Traverse();

    // 输出扫描结果
    const auto &files = dir_scanner.GetFiles();
    std::cout << "Files found in directory " << base_dir << ":\n";
    for (const auto &file : files) {
      std::cout << file << "\n";
    }

  } catch (const exception &e) {
    cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}