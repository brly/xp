#ifndef BASE_METHOD_H_
#define BASE_METHOD_H_

#include "constant.h"
#include "svm_wrapper.h"
#include "method.h"

// 元となる手法
// wq を導出するため、学習サンプルを1万必要とする

class BaseMethod : public Method {
  SvmWrapper svm;
  std::vector<double> wq;
  std::string query_;

  void init_svm_problem();
  void set_positive_svm();
  void set_negative_svm();
 public:
  explicit BaseMethod(const std::string& query) :
      svm(1, NULL, false),
      wq(kTotalDim, 0),
      query_(query) {}
  void run();
  void run(std::vector<std::string>& ranking);
};

#endif // BASE_METHOD_H_
