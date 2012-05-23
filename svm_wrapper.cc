#include "hog.h"
#include "svm_wrapper.h"

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <cassert>
#include <cstdio>
#include <vector>
#include <utility>

namespace {

// 学習に必要なパラメータインスタンス
svm_parameter param;
svm_problem problem;

// 画像ファイルの名前リスト
const std::string kPositiveSampleFile = "POSITIVE_sample";
const std::string kNegativeSampleFile = "NEGATIVE_sample";
const std::string kPositiveDatabaseFile = "POSITIVE_database";
const std::string kNegativeDatabaseFile = "NEGATIVE_database";

// 定数
// svm_problemに設定するインスタンス数
const int kSampleL = 12;
const int kDatabaseL = 11;

////////////////////////////////////////////////////////////////////////////////
// svm_parameterを初期化する

void init_svm_param() {
  param.svm_type = C_SVC;
  param.kernel_type = LINEAR;
  param.degree = 3;
  param.gamma = 0;
  param.coef0 = 0;
  param.nu = 0.5;
  param.cache_size = 100;
  param.C = 1;
  param.eps = 1e-3;
  param.p = 0.1;
  param.shrinking = 1;
  param.probability = 1;
  param.nr_weight = 0;
  param.weight_label = NULL;
  param.weight = NULL;
}

////////////////////////////////////////////////////////////////////////////////
// svm_problem を初期化する
// 1. ポジティブ、ネガティブの画像の特徴をすべて求める。
// 2. svm_problemに設定する
// 現在は画像リストの名前を仮定している
// さらに、問題のインスタンス数 l が自明であるので、適当に事前に設定する
// そのうち、SQLiteとかで管理する..
void init_svm_problem() {
  problem.l = kSampleL;
  problem.y = new double[kSampleL];
  problem.x = new svm_node *[kSampleL];
  int idx = 0; 
  std::string line;
  
  // Positive sample
  std::ifstream positive_set_stream(kPositiveSampleFile.c_str());
  while (std::getline(positive_set_stream, line)) {
    std::shared_ptr<double> feat(Hog::get_feature(line));
    problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      problem.x[idx][i].index = i + 1;
      problem.x[idx][i].value = feat.get()[i];
    }
    problem.x[idx][kTotalDim].index = -1;
    problem.y[idx] = 1;
    ++idx;
  }
  // Negative sample
  std::ifstream negative_set_stream(kNegativeSampleFile.c_str());
  while (std::getline(negative_set_stream, line)) {
    std::shared_ptr<double> feat(Hog::get_feature(line));
    problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      problem.x[idx][i].index = i + 1;
      problem.x[idx][i].value = -(feat.get()[i]);
    }
    problem.x[idx][kTotalDim].index = -1;
    problem.y[idx] = -1;
    ++idx;
  }
  // Query
  // クエリファイルが存在するか確認する
  std::shared_ptr<double> feat(Hog::get_feature("easy_test/circle_1.jpg"));
  problem.x[idx] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    problem.x[idx][i].index = i + 1;
    problem.x[idx][i].value = feat.get()[i];
  }
  problem.x[idx][kTotalDim].index = -1;
  problem.y[idx] = 1;

}

void eval_vector(const std::vector<double>& w) {
  std::string line;
  std::ifstream posi_str(kPositiveDatabaseFile.c_str()),
      nega_str(kNegativeDatabaseFile.c_str());
  typedef std::pair<double, std::string> ResultPair;
  std::vector<ResultPair> result;
  // positive 
  while (std::getline(posi_str, line)) {
    std::shared_ptr<double> feat(Hog::get_feature(line));
    double sum = 0;
    for (int i = 0; i < kTotalDim; ++i)
      sum += w[i] * feat.get()[i];
    result.push_back(ResultPair(sum, line));
  }
  // negative
  while (std::getline(nega_str, line)) {
    std::shared_ptr<double> feat(Hog::get_feature(line));
    double sum = 0;
    for (int i = 0; i < kTotalDim; ++i)
      sum += w[i] * feat.get()[i];
    result.push_back(ResultPair(sum, line));
  }
  sort(result.begin(), result.end(), std::greater<ResultPair>());
  // output result
  for (unsigned int i = 0; i < result.size(); ++i) {
    ResultPair rp = result.at(i);
    printf("%3d : %-30s %lf\n", i, rp.second.c_str(), rp.first);
  }
}

void print_null(const char *s) {}

}; // empty namespace

void init_svm() {
  init_svm_param();
  init_svm_problem();
  svm_set_print_string_function(&print_null);
}

////////////////////////////////////////////////////////////////////////////////
// svm_problem パラメータの内部インスタンスを開放する
void dispose_svm() {
  if (problem.y)
    delete[] problem.y;
  
  if (problem.x) {
    for (int i = 0; i < kSampleL; ++i)
      if (problem.x[i])
        delete[] problem.x[i];
    delete[] problem.x;
  }
}

void get_model() {
  if (svm_check_parameter(&problem, &param) != NULL)
    assert(false);

  svm_model *model = svm_train(&problem, &param);
 
  std::vector<double> w(kTotalDim, 0);
  for (int h = 0; h < model->l; ++h) 
    for (int i = 0; i < kTotalDim; ++i) 
      w[i] += model->SV[h][i].value * model->sv_coef[0][h];
  

  eval_vector(w);
  
  svm_free_and_destroy_model(&model);
}
