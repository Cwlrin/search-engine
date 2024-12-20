#include <iostream>

#include "page/Configuration.h"
#include "page/DirScanner.h"

int main() {
  try {
    // 更改当前目录到项目根目录
    fs::current_path("/home/ichika/SearchEngine");

    Configuration config("conf/corpus_config.json");

    // 使用解析后的配置
    const string pages_path = config.GetPagesPath();
    const auto &stop_words_cn = config.GetStopWordsCn();
    const auto &stop_words_en = config.GetStopWordsEn();

    cout << "Pages path: " << pages_path << endl;
    cout << "Loaded " << stop_words_cn.size() << " Chinese stop words."
              << endl;
    cout << "Loaded " << stop_words_en.size() << " English stop words."
              << endl;

  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }

  return 0;
}