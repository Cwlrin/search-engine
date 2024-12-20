#include <iostream>

#include "page/Configuration.h"
#include "page/DirScanner.h"

int main() {
  try {
    // 更改当前目录到项目根目录
    fs::current_path("/home/ichika/SearchEngine");

    // 加载配置
    Configuration config("conf/corpus_config.json");

    // 创建 DirScanner 对象并扫描
    DirScanner scanner(config);
    scanner();

    // 输出扫描结果
    const auto &files = scanner.Files();
    cout << "Scanned files:" << endl;
    for (const auto &file : files) {
      cout << file << endl;
    }

  } catch (const exception &e) {
    cerr << "Error: " << e.what() << std::endl;
  }

  return 0;
}