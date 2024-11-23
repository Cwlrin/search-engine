#include "dictionary/SplitToolCppJieba.h"

const char *const DICT_PATH = "../res/cppjieba/jieba.dict.utf8";
const char *const HMM_PATH = "../res/cppjieba/hmm_model.utf8";
const char *const USER_DICT_PATH = "../res/cppjieba/user.dict.utf8";
const char *const IDF_PATH = "../res/cppjieba/idf.utf8";
const char *const STOP_WORD_PATH = "../res/cppjieba/stop_words.utf8";

// 饿汉模式
cppjieba::Jieba SplitToolCppJieba::cuttor_(DICT_PATH, HMM_PATH, USER_DICT_PATH,
                                           IDF_PATH, STOP_WORD_PATH);

vector<string> SplitToolCppJieba::Cut(const string &sentence) {
  // 切割中文单词
  vector<string> words;
  cuttor_.Cut(sentence, words, true);

  // 返回切割结果
  return words;
}
