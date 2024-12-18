#include "bitsc++.h"
#include "dictionary/DicProducer.h"
#include "dictionary/SplitToolCppJieba.h"

int main() {
  // 更改工作目录到项目根目录
  fs::current_path("/home/ichika/SearchEngine");
  try {
    Configuration config("conf/corpus_config.json");

    // 构建中文词典
    SplitToolCppJieba split_tool;
    DictProducer cn_dict_producer(config, &split_tool);
    cn_dict_producer.BuildCnDict();
    cn_dict_producer.StoreDict();

    // 构建英文词典
    DictProducer en_dict_producer(config);
    en_dict_producer.BuildEnDict();
    en_dict_producer.StoreDict();

  } catch (const exception &e) {
    cerr << "Error: " << e.what() << endl;
  }
  return 0;
}