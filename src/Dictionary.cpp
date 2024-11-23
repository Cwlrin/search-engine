#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  try {
    // 更改工作目录到项目根目录
    fs::current_path("/home/ichika/SearchEngine");
    // 测试英文语料库
    // 读取语料
    const string config_file_path = "conf/corpus_config.json";
    DictProducer english_dict_producer(config_file_path);

    cout << "English corpus files:" << endl;
    for (const auto& file : english_dict_producer.GetFiles()) {
      cout << file << endl;
    }

    // 清洗语料文件
    cout << "Cleaning corpus files..." << endl;
    for (const auto& file : english_dict_producer.GetFiles()) {
      DictProducer::ReadEnFile(file);  // 清洗文件内容
    }
    cout << "Corpus cleaning completed." << endl;
    // 构建词典
    cout << "Building English dictionary..." << endl;
    english_dict_producer.BuildEnDict();

    // 保存词典到文件
    cout << "Saving dictionary to file..." << endl;
    english_dict_producer.Store();

  } catch (const std::exception& e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}