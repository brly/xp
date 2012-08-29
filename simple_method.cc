#include "simple_method.h"
#include "hog.h"
#include "simple_method.h"
#include "svm.h"
#include "util.h"
#include "constant.h"
#include "random_function.h"
#include "search_database.h"
#include "timer.h"

#include <algorithm>
#include <fstream>

void SimpleMethod::set_svm_problem_impl(const std::vector<std::string>& files,
                                        int& idx, const int flag_value) {
  for (unsigned i = 0; i < files.size(); ++i) {
    // Hog hog(files[i].c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    std::vector<double> hog;
    Util::read_vector_data(files[i], hog);
    svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      svm.problem.x[idx][i].index = i + 1;
      svm.problem.x[idx][i].value = flag_value * hog[i];
    }
    svm.problem.x[idx][kTotalDim].index = -1;
    svm.problem.y[idx] = flag_value;
    ++idx;
  }
}

void SimpleMethod::set_positive_to_svm_problem(int& idx) {
  std::vector<std::string> vs;
<<<<<<< HEAD
=======
  // vs.push_back(kFeatureVectorDir + "/circle_0");
>>>>>>> origin/master
  query_ = kFeatureVectorDir + query_.substr(query_.find('/'));
  query_.erase(query_.size() - 4);

  if (!Util::is_exist(query_.c_str())) assert(false);
  
  vs.push_back(query_);
  set_svm_problem_impl(vs, idx, 1);
}

void SimpleMethod::set_negative_to_svm_problem(int& idx) {
  std::vector<std::string> vs;
<<<<<<< HEAD
  Util::get_file_list(kFeatureVectorDir + "/negativeImages", vs, true);
  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());
  vs.erase(vs.begin() + k_, vs.end());
=======
  Util::get_file_list(kFeatureVectorDir, vs, true);
  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());
  vs.erase(vs.begin() + 50, vs.end());
>>>>>>> origin/master

  set_svm_problem_impl(vs, idx, -1);
}

void SimpleMethod::init_svm_problem() {
  // 計算時間が少ないであろうサンプル数(ネガティブのみ計上)を設定
<<<<<<< HEAD
  const int kSampleL = k_ + 1;
=======
  const int kSampleL = 50 + 1;
>>>>>>> origin/master
  svm.problem.l = kSampleL;
  svm.problem.y = new double[kSampleL];
  svm.problem.x = new svm_node * [kSampleL];
  
  // ポジティブサンプルの設定
  int idx = 0;
  set_positive_to_svm_problem(idx);

  // ネガティブサンプルの設定
  set_negative_to_svm_problem(idx);
}

void SimpleMethod::run() {
  std::vector<double> wq;
  {
    Timer timer("simple method");
    this->init_svm_problem();
    wq = svm.get_weight_vector();
  }
<<<<<<< HEAD
  
=======

>>>>>>> origin/master
  SearchDatabase::search(wq);
}

void SimpleMethod::run(std::vector<std::string>& ranking) {
  std::vector<double> wq;
  {
<<<<<<< HEAD
    Timer timer("simple");
=======
    Timer timer("simple method");
>>>>>>> origin/master
    this->init_svm_problem();
    wq = svm.get_weight_vector();
  }

<<<<<<< HEAD
  // SearchDatabase::search(wq, ranking, 10);
  SearchDatabase::search(wq, ranking, 0);
=======
  SearchDatabase::search(wq, ranking, 10);
>>>>>>> origin/master
}
