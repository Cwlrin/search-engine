#include "bitsc++.h"
#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");
  try {

    // 测试英文语料库
//    // 读取语料
//    string configFilePath = "conf/corpus_config.json";
//    DictProducer englishDictProducer(configFilePath);
//
//    cout << "English corpus files:" << endl;
//    for (const auto& file : englishDictProducer.getFiles()) {
//      cout << file << endl;
//    }
//
//    // 清洗语料文件
//    cout << "Cleaning corpus files..." << endl;
//    for (const auto& file : englishDictProducer.getFiles()) {
//      DictProducer::readEnFile(file);  // 清洗文件内容
//    }
//    cout << "Corpus cleaning completed." << endl;
//    // 构建词典
//    cout << "Building English dictionary..." << endl;
//    englishDictProducer.buildEnDict();
//
//    // 保存词典到文件
//    cout << "Saving dictionary to file..." << endl;
//    englishDictProducer.store();
    // 创建中文词典生成器
    string configFilePath = "conf/corpus_config.json";
    SplitToolCppJieba splitTool;
    DictProducer chineseDictProducer(configFilePath, &splitTool);

    // 清洗并分词中文语料文件
    cout << "Processing and segmenting Chinese corpus files..." << endl;
    for (const auto &file : chineseDictProducer.GetFiles()) {
      chineseDictProducer.ReadCnFile(file);
    }

    // 构建中文词典
    cout << "Building Chinese dictionary..." << endl;
    chineseDictProducer.BuildCnDict();

    // 保存词典到文件
    cout << "Saving Chinese dictionary to file..." << endl;
    chineseDictProducer.Store();

  } catch (const std::exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}