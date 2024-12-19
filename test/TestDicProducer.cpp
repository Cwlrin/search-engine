#include "bitsc++.h"
#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");

  // 清空索引文件
  ofstream ofs("res/Index.dat", ios::out | ios::binary);

  try {

    Configuration config("conf/corpus_config.json");

    // ================================
    // 测试中文词典生成
    cout << "Starting Chinese dictionary generation..." << endl;
    SplitToolCppJieba split_tool;
    DictProducer cn_dict_producer(config, &split_tool);
    cn_dict_producer.BuildCnDict();
    cn_dict_producer.StoreCnDict();
    cn_dict_producer.CreateIndex();
    cn_dict_producer.StoreIndex();

    cout << "Chinese dictionary and index generation completed." << endl;

    // ================================
    // 测试英文词典生成
    cout << "Starting English dictionary generation..." << endl;
    DictProducer en_dict_producer(config);
    en_dict_producer.BuildEnDict();
    en_dict_producer.StoreEnDict();
    en_dict_producer.CreateIndex();
    en_dict_producer.StoreIndex();

    cout << "English dictionary and index generation completed." << endl;

    // ================================
    // 验证生成结果
    cout << "Verifying generated dictionaries and index files..." << endl;
    ifstream cn_dict("res/CnDict.dat");
    ifstream en_dict("res/EnDict.dat");
    ifstream index("res/Index.dat");

    if (cn_dict && en_dict && index) {
      cout << "All files have been successfully generated." << endl;
    } else {
      cerr << "Error: One or more files could not be opened!" << endl;
    }

  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}