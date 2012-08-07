#include "base_method.h"
#include "hog.h"
#include "random_function.h"
#include "util.h"
#include "timer.h"
#include "search_database.h"

namespace {
const int kL = 11;
};

void BaseMethod::init_svm_problem() {
  // problem の初期設定
  svm.problem.l = kL;
  svm.problem.y = new double[kL];
  svm.problem.x = new svm_node * [kL];

  // ポジティブの設定
  set_positive_svm();

  // ネガティブの設定
  set_negative_svm();
}

void BaseMethod::set_positive_svm() {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    svm.problem.x[0][i].index = i + 1;
    svm.problem.x[0][i].value = h[i];
  }
  svm.problem.x[0][kTotalDim].index = -1;
  svm.problem.y[0] = 1;
}

void BaseMethod::set_negative_svm() {
  std::vector<std::string> files;
  Util::get_file_list(kFeatureVectorDir, files, true);
  std::random_shuffle(files.begin(), files.end(), RandomFunction());

  for (int i = 1; i < kL; ++i) {
    std::vector<double> vd;
    Util::read_vector_data(files[i-1], vd);
    // svm へ登録
    svm.problem.x[i] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[i][j].index = j + 1;
      svm.problem.x[i][j].value = -vd[j];
    }
    svm.problem.x[i][kTotalDim].index = -1;
    svm.problem.y[i] = -1;
  }
}

void BaseMethod::run() {
  Timer timer("base method");
  this->init_svm_problem();
  wq = svm.get_weight_vector();
  SearchDatabase::search(wq);
}

void BaseMethod::run(std::vector<std::string>& ranking) {
  Timer timer("base method");
  this->init_svm_problem();
  wq = svm.get_weight_vector();
  SearchDatabase::search(wq, ranking, 10);
}
