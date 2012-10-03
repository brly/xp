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
  std::string query_;
  const int k_;

  void init_svm_problem();
  
  // TODO: 関数名を直す
  void set_svm_problem_impl(const std::vector<std::string>& files,
                            int& idx, const int flag_value);
  void set_positive_to_svm_problem(int& idx);
  void set_negative_to_svm_problem(int& idx);
 public:
  SimpleMethod(const std::string& query, const int k = 50)
      : svm(1, NULL, false), query_(query), k_(k) {}
  void run();
  void run(std::vector<std::string>& ranking);
};

#endif // SIMPLE_METHOD_H_
