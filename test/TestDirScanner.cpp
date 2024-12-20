#include <iostream>

#include "page/Configuration.h"
#include "page/DirScanner.h"

int main() {
  try {
    // 更改当前目录到项目根目录
    fs::current_path("/home/ichika/SearchEngine");
    // 加载配置文件
    Configuration config("conf/corpus_config.json");
    // 获取 pages 路径
    string pages_path = config.GetPagesPath();
    if (pages_path.empty()) {
      throw runtime_error("Error: Pages path is not defined in the configuration.");
    }

    cout << "Pages path loaded: " << pages_path << endl;

    // 创建 DirScanner 对象
    DirScanner dir_scanner(pages_path);

    // 开始扫描目录
    dir_scanner.Traverse();

    // 获取文件列表
    const auto &files = dir_scanner.GetFiles();

    // 输出文件列表
    cout << "Scanned Files:" << endl;
    for (const auto &file : files) {
      cout << file << endl;
    }

    // 检查扫描结果
    if (files.empty()) {
      cerr << "Error: No files found in directory: " << pages_path
                << endl;
      return -1;
    }

    cout << "Test Passed: Successfully scanned " << files.size()
              << " files." << endl;
  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }

  return 0;
}