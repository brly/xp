#ifndef ASSEMBLING_METHOD_H_
#define ASSEMBLING_METHOD_H_

// assembling_method.h
// ランダムに代表となるものを決め、それらについて重みベクトルを求める
// その後、代表のみを用いてクエリに関する重みベクトルを導出する

#include "method.h"
#include "svm_wrapper.h"
#include "hog.h"

#include <vector>
#include <string>

class AssemblingMethod : public Method {
  SvmWrapper svm;

  void init_svm_problem();

  void set_represents(std::vector<std::string>& represents,
                      const char *file_name);
  
  void get_svm_each_represent(SvmWrapper& temp_svm,
                              const std::string& represent);
  
 public:
  AssemblingMethod(int argc, char**argv) : svm(argc, argv, false) {}
  void run();
};

#endif // ASSEMBLING_METHOD_H_
