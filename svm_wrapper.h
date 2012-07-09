// LIBSVMとの仲立ちを行う
// 達成する事
// 1. svm_train()を呼び出すための事前設定
//    事前設定(param, problem)には、画像ファイルの情報が必要
// 2. svm_train()を呼ぶ

#ifndef SVM_WRAPPER_H
#define SVM_WRAPPER_H

#include "svm.h"

#include <vector>
#include <cstdlib>

// インターフェース

class SvmWrapper {
  void init_svm_parameter();
  void init_svm_problem();
  void init_svm_problem_dynamic(const char* name);
  void eval_vector(const std::vector<double>& w, const double rho);
  void get_model();
  void dispose_svm();
  void for_presentation(const std::vector<double>& w);
 public:
  SvmWrapper(int argc = 0, char**argv = NULL);
  ~SvmWrapper();
  void run();
};

#endif // SVM_WRAPPER_H
