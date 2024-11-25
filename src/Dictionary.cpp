#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");

  try {
    const string config_file_path = "conf/corpus_config.json";

    // ==============================
    // 构建中文词典
    // ==============================
    cout << "Starting Chinese dictionary construction..." << endl;

    // 创建分词工具
    SplitToolCppJieba split_tool;

    // 创建中文词典生成器
    DictProducer chinese_dict_producer(config_file_path, &split_tool);

    // 清洗并分词中文语料文件
    cout << "Processing and segmenting Chinese corpus files..." << endl;
    for (const auto& file : chinese_dict_producer.GetFiles()) {
      cout << "Processing file: " << file << endl;
      chinese_dict_producer.ReadCnFile(file);  // 清洗并分词
    }

    // 构建中文词典
    cout << "Building Chinese dictionary..." << endl;
    chinese_dict_producer.BuildCnDict();

    // 保存中文词典到文件
    const string cn_dict_path = "res/CnDict.dat";
    cout << "Saving Chinese dictionary to file..." << endl;
    chinese_dict_producer.StoreDict();

    // ==============================
    // 构建英文词典
    // ==============================
    cout << "Starting English dictionary construction..." << endl;

    // 创建英文词典生成器
    DictProducer english_dict_producer(config_file_path);

    // 清洗英文语料文件
    cout << "Cleaning English corpus files..." << endl;
    for (const auto& file : english_dict_producer.GetFiles()) {
      cout << "Processing file: " << file << endl;
      DictProducer::ReadEnFile(file);  // 清洗英文文件
    }

    // 构建英文词典
    cout << "Building English dictionary..." << endl;
    english_dict_producer.BuildEnDict();

    // 保存英文词典到文件
    const string en_dict_path = "res/EnDict.dat";
    cout << "Saving English dictionary to file..." << endl;
    english_dict_producer.StoreDict();

    // ==============================
    // 构建统一索引
    // ==============================
    cout << "Creating unified index for dictionaries..." << endl;

    // 加载中文词典
    chinese_dict_producer.LoadDict(cn_dict_path);

    // 加载英文词典
    english_dict_producer.LoadDict(en_dict_path);

    // 创建索引（包含中文和英文词典）
    chinese_dict_producer.CreateIndex();
    english_dict_producer.CreateIndex();

    // 合并中文和英文索引
    cout << "Saving unified index to file..." << endl;
    map<string, set<int>> combined_index = chinese_dict_producer.GetIndex();
    for (const auto& [character, word_set] : english_dict_producer.GetIndex()) {
      combined_index[character].insert(word_set.begin(), word_set.end());
    }

    // 保存索引
    chinese_dict_producer.StoreIndex(combined_index);

    cout << "All dictionaries and indices have been successfully built and saved." << endl;

  } catch (const exception& e) {
    cerr << "Error: " << e.what() << endl;
  }

  return 0;
}