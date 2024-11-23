#include "bitsc++.h"
#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");

  try {
    // 配置文件路径
    const string config_file_path = "conf/corpus_config.json";

    // 创建中文分词工具
    SplitToolCppJieba split_tool;

    // 创建中文词典生成器
    DictProducer chinese_dict_producer(config_file_path, &split_tool);

    // 清洗并分词中文语料文件
    cout << "Processing and segmenting Chinese corpus files..." << endl;
    for (const auto &file : chinese_dict_producer.GetFiles()) {
      cout << "Processing file: " << file << endl;
      chinese_dict_producer.ReadCnFile(file);  // 读取并清洗中文语料文件
    }

    // 构建中文词典
    cout << "Building Chinese dictionary..." << endl;
    chinese_dict_producer.BuildCnDict();

    // 保存中文词典到文件
    cout << "Saving Chinese dictionary to file..." << endl;
    chinese_dict_producer.Store();

    // 创建英文词典生成器
    DictProducer english_dict_producer(config_file_path);

    // 清洗英文语料文件
    cout << "Cleaning English corpus files..." << endl;
    for (const auto &file : english_dict_producer.GetFiles()) {
      cout << "Processing file: " << file << endl;
      DictProducer::ReadEnFile(file);  // 读取并清洗英文语料文件
    }

    // 构建英文词典
    cout << "Building English dictionary..." << endl;
    english_dict_producer.BuildEnDict();

    // 保存英文词典到文件
    cout << "Saving English dictionary to file..." << endl;
    english_dict_producer.Store();

    cout << "All dictionaries have been successfully built and saved." << endl;

  } catch (const std::exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
}