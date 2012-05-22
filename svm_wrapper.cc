#include "hog.h"
#include "svm_wrapper.h"

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <cassert>
#include <cstdio>

namespace {

// 学習に必要なパラメータインスタンス
svm_parameter param;
svm_problem problem;

// 画像ファイルの名前リスト
const std::string kPositiveSet = "POSITIVE";
const std::string kNegativeSet = "NEGATIVE";

// 定数
// svm_problemに設定するインスタンス数
const int kL = 3;

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
  problem.l = kL;
  problem.y = new double[kL];
  problem.x = new svm_node *[kL];
  int idx = 0; 
  
  // POSITIVE
  {
    std::ifstream positive_set_stream(kPositiveSet.c_str());
    std::string line;
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
  }
  // NEGATIVE
  {
    std::ifstream negative_set_stream(kNegativeSet.c_str());
    std::string line;
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
  delete[] problem.y;
  for (int i = 0; i < kL; ++i) delete[] problem.x[i];
  delete[] problem.x;
}

void get_model() {
  if (svm_check_parameter(&problem, &param) != NULL)
    assert(false);
  
  svm_model *model = svm_train(&problem, &param);
  printf("model->l is %d\n", model->l);
  printf("model->nr_class is %d\n", model->nr_class);
  for (int i = 0; i < 10; ++i) {
    printf("SV %d is %lf (idx is %d)\n", i+1, model->SV[0][i].value, model->SV[0][i].index);
  }

  for (int i = 0; i < model->l; ++i)
    printf("sv_coef test %lf\n", model->sv_coef[0][i]);
  
  svm_free_and_destroy_model(&model);
}
