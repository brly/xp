#ifndef SIMPLE_METHOD_H_
#define SIMPLE_METHOD_H_

// simple_method.h
// 単純な手法. ネガティブサンプルを与え、その内からランダムに選択して
// ネガティブサンプルの数を少数に抑えることで、計算時間を落とす。

#include "method.h"
#include "svm_wrapper.h"

#include <string>
#include <vector>

class SimpleMethod : public Method {
  SvmWrapper svm;

  void init_svm_problem();
  
  // TODO: 関数名を直す
  void set_svm_problem_impl(const std::vector<std::string>& files,
                            int& idx, const int flag_value);
  void set_positive_to_svm_problem(const char* negative_file, int& idx);
  void set_negative_to_svm_problem(const char* positive_file, int& idx);
 public:
  SimpleMethod(int argc, char**argv) : svm(argc, argv, false) {}
  void run();
};

#endif // SIMPLE_METHOD_H_
