#include "DicProducer.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");
  // 测试英文语料库
  try {
    // 读取语料
    string configFilePath = "conf/corpus_config.json";
    DictProducer englishDictProducer(configFilePath);

    cout << "English corpus files:" << endl;
    for (const auto& file : englishDictProducer.getFiles()) {
      cout << file << endl;
    }

    // 清洗语料文件
    cout << "Cleaning corpus files..." << endl;
    for (const auto& file : englishDictProducer.getFiles()) {
      DictProducer::readFile(file);  // 清洗文件内容
    }
    cout << "Corpus cleaning completed." << endl;
    // 构建词典
    cout << "Building English dictionary..." << endl;
    englishDictProducer.buildEnDict();

    // 保存词典到文件
    cout << "Saving dictionary to file..." << endl;
    englishDictProducer.store();

  } catch (const std::exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}