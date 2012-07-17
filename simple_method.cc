#include "simple_method.h"
#include "hog.h"
#include "simple_method.h"
#include "svm.h"
#include "constant.h"
#include "random_function.h"

#include <algorithm>
#include <fstream>

void SimpleMethod::set_svm_problem_impl(const std::vector<std::string>& files,
                                        int& idx, const int flag_value) {
  for (unsigned i = 0; i < files.size(); ++i) {
    Hog hog(files[i].c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    svm.problem.x[idx] = new svm_node[kTotalDim];
    for (int i = 0; i < kTotalDim; ++i) {
      svm.problem.x[idx][i].index = i + 1;
      svm.problem.x[idx][i].value = hog[i];
    }
    svm.problem.x[idx][kTotalDim].index = -1;
    svm.problem.y[idx] = flag_value;
    ++idx;
  }
}

void SimpleMethod::set_positive_to_svm_problem(const char* positive_file,
                                               int& idx) {
  // set_svm_problem_impl(positive_file, idx, 1);
  std::ifstream positive_set_stream(positive_file);
  std::string line;
  std::vector<std::string> vs;
  while (std::getline(positive_set_stream, line)) 
    vs.push_back(line);
  set_svm_problem_impl(vs, idx, 1);
}

void SimpleMethod::set_negative_to_svm_problem(const char* negative_file,
                                               int& idx) {
  std::ifstream negative_set_stream(negative_file);
  std::string line;
  std::vector<std::string> vs;
  while (std::getline(negative_set_stream, line))
    vs.push_back(line);
  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());
  vs.erase(vs.begin() + 50, vs.end());
  set_svm_problem_impl(vs, idx, -1);
}

void SimpleMethod::init_svm_problem() {
  // 計算時間が少ないであろうサンプル数(ネガティブのみ計上)を設定
  const int kSampleL = 50 + 1;
  svm.problem.l = kSampleL;
  svm.problem.y = new double[kSampleL];
  svm.problem.x = new svm_node * [kSampleL];
  
  // ポジティブサンプルの設定
  int idx = 0;
  set_positive_to_svm_problem("POSITIVE_sample", idx);

  // ネガティブサンプルの設定
  set_negative_to_svm_problem("NEGATIVE_sample", idx);
}

void SimpleMethod::run() {
  class Timer {
    std::string process_name_;
    clock_t begin_;
   public:
    explicit Timer(const char* process_nane) : process_name_(process_nane),
                                               begin_(clock()) {}
    ~Timer() {
      printf("%s - %f sec\n",
             process_name_.c_str(),
             static_cast<double>(clock() - begin_) / CLOCKS_PER_SEC);
    }
  };
  Timer timer("simple method");
  this->init_svm_problem();
  svm.run();
}
