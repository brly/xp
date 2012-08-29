#include "c_method.h"
#include "hog.h"
#include "random_function.h"
#include "search_database.h"
#include "timer.h"
#include "util.h"

#include <cassert>

void CMethod::init_svm_problem() {
  if (kBeta > 100 || kBeta < 0) {
    assert(false);
  }

  // svm の初期設定
  svm.problem.l = kM + 1;
  svm.problem.x = new svm_node * [kM + 1];
  svm.problem.y = new double[kM+1];

  // ポジティブの設定
  set_positive_problem();

  // ネガティブの設定
  const int kRandom = kM * (kBeta / 100.0);
  const int kNear = kM - kRandom;
  int idx = 1;

  // 1. データベースからランダムに選択
  set_negative_problem_random(kRandom, idx);

  // 2. 内積の値が大きいものから選択
  set_negative_problem_near(kNear, idx);

  // SVM で重みベクトルを計算
  wq = svm.get_weight_vector();
}

void CMethod::set_positive_problem() {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY,
        kOrientation);
  
  svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    svm.problem.x[0][i].index = i + 1;
    svm.problem.x[0][i].value = q[i] = h[i];
  }
  svm.problem.x[0][kTotalDim].index = -1;
  svm.problem.y[0] = 1;
}

void CMethod::set_negative_problem_random(const int kRandom, int& idx) {
  std::vector<std::string> samples;
  Util::get_file_list(kFeatureVectorDir, samples, true);
  std::random_shuffle(samples.begin(), samples.end(), RandomFunction());

  for (int i = 0; i < kRandom; ++i) {
    std::vector<double> vd;
    Util::read_vector_data(samples[i], vd);

    svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[idx][j].index = j + 1;
      svm.problem.x[idx][j].value = -vd[j];
    }
    svm.problem.x[idx][kTotalDim].index = -1;
    svm.problem.y[idx] = -1;
    ++idx;
  }
}

void CMethod::set_negative_problem_near(const int kNear, int& idx) {
  // 内積による検索結果を取得
  std::vector<std::string> result;
  SearchDatabase::search(q, result, kNear);

  // 検索結果の要素を追加する
  for (int i = 0; i < kNear; ++i) {
    std::vector<double> vd;
    Util::read_vector_data(result[i], vd);

    svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[idx][j].index = j + 1;
      svm.problem.x[idx][j].value = -vd[j];
    }
    svm.problem.x[idx][kTotalDim].index = -1;
    svm.problem.y[idx] = -1;
    ++idx;
  }
}

void CMethod::run() {
  {
    Timer timer("CMethod");
    this->init_svm_problem();
  }
  SearchDatabase::search(wq);
}

void CMethod::run(std::vector<std::string>& ranking) {
  {
    Timer timer("CMethod");
    this->init_svm_problem();
  }
  SearchDatabase::search(wq, ranking, 0);
}
