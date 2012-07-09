#include "hog.h"
#include "svm_wrapper.h"
#include "visualize_hog.h"
#include "constant.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <string>
#include <fstream>
#include <memory>
#include <cassert>
#include <cstdio>
#include <vector>
#include <utility>

namespace {

// 画像ファイルの名前リスト
const std::string kPositiveSampleFile = "POSITIVE_sample";
const std::string kNegativeSampleFile = "NEGATIVE_sample";
const std::string kPositiveDatabaseFile = "POSITIVE_database";
const std::string kNegativeDatabaseFile = "NEGATIVE_database";

// 定数
// svm_problemに設定するインスタンス数
// CAUTION :: 手動で設定する必要がある
// 正常な設定が行われていない(POSITIVE, NEGATIVE とのデータ数が異なる) 場合、
// メモリの動的確保に失敗しSIGSEGVシグナルでプログラムが停止する可能性があります
const int kSampleL = 10001;

void print_null(const char *s) {}

}; // empty namespace

////////////////////////////////////////////////////////////////////////////////
// svm_parameterを初期化する

void SvmWrapper::init_svm_parameter() {
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

// Legacy code 1
////////////////////////////////////////////////////////////////////////////////
// svm_problem を初期化する
// 1. ポジティブ、ネガティブの画像の特徴をすべて求める。
// 2. svm_problemに設定する
// 現在は画像リストの名前を仮定している
// さらに、問題のインスタンス数 l が自明であるので、適当に事前に設定する
// そのうち、SQLiteとかで管理する..
void SvmWrapper::init_svm_problem() {
  printf("Total Sample (Positive + Negative) is %d\n", kSampleL);
  problem.l = kSampleL;
  problem.y = new double[kSampleL];
  problem.x = new svm_node *[kSampleL];
  int idx = 0; 
  std::string line;
  
  // Positive sample
  std::ifstream positive_set_stream(kPositiveSampleFile.c_str());
  while (std::getline(positive_set_stream, line)) {
    Hog hog(line.c_str(), 5, 3, 40, 40, 9);
    problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      problem.x[idx][i].index = i + 1;
      problem.x[idx][i].value = hog[i];
    }
    problem.x[idx][kTotalDim].index = -1;
    problem.y[idx] = 1;
    ++idx;
  }
  // Negative sample
  std::ifstream negative_set_stream(kNegativeSampleFile.c_str());
  while (std::getline(negative_set_stream, line)) {
    Hog hog(line.c_str(), 5, 3, 40, 40, 9);
    // cv::Mat check = VisualizeHog::generate_mat(feat);
    // cv::namedWindow("check", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    // cv::imshow("check", check);
    // cv::waitKey(0);
    problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      problem.x[idx][i].index = i + 1;
      problem.x[idx][i].value = -hog[i];
    }
    problem.x[idx][kTotalDim].index = -1;
    problem.y[idx] = -1;
    ++idx;
  }
}

// Legacy Code 2
void SvmWrapper::init_svm_problem_dynamic(const char *name) {
  int k_sample_l = 0;
  // Negative Sample load & calc number of sample
  int negative_n, negative_dim;
  std::ifstream negative_stream(name);
  negative_stream >> negative_n >> negative_dim;
  k_sample_l += negative_n;

  // print error
  fprintf(stderr, "Total sample size is %d\n", k_sample_l);

  // Set positive and negative
  // CAUTION !! currently, idx 0 is positive and other is negative.
  // About above can be checked with - 
  // 'default result(run with non given support vector)'.
  problem.l = k_sample_l;
  problem.y = new double[problem.l];
  problem.x = new svm_node * [problem.l];
  int idx = 0;
  std::string line;
  
  for (int i = 0; i < negative_n; ++i) {
    problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      negative_stream >> problem.x[idx][j].value;
      problem.x[idx][j].index = i + 1;
    }
    problem.x[idx][kTotalDim].index = -1;
    if (i == 0)
      problem.y[idx] = 1;
    else
      problem.y[idx] = -1;
    ++idx;
  }
}

void SvmWrapper::eval_vector(const std::vector<double>& w, const double rho) {
  std::string line;
  std::ifstream posi_str(kPositiveDatabaseFile.c_str()),
      nega_str(kNegativeDatabaseFile.c_str());
  typedef std::pair<double, std::string> ResultPair;
  std::vector<ResultPair> result;
  // positive 
  while (std::getline(posi_str, line)) {
    Hog hog(line.c_str(), 5, 3, 40, 40, 9);
    double sum = 0;
    for (int i = 0; i < kTotalDim; ++i)
      sum += w[i] * hog[i];
    result.push_back(ResultPair(sum - rho, line));
    printf("%s : %f\n", line.c_str(), sum - rho);
  }
  // negative
  while (std::getline(nega_str, line)) {
    // std::shared_ptr<double> feat(Hog::get_feature(line));
    Hog hog(line.c_str(), 5, 3, 40, 40, 9);
    double sum = 0;
    for (int i = 0; i < kTotalDim; ++i)
      sum += w[i] * hog[i];
    result.push_back(ResultPair(sum - rho, line));
  }

  const unsigned kTop = 10;
  printf("result size is %d\n", result.size());
  // std::sort(result.begin(), result.end(), std::greater<ResultPair>());
  std::partial_sort(result.begin(), result.begin() + kTop, result.end(),
                    std::greater<ResultPair>());
  // output result
  for (unsigned i = 0; i < kTop; ++i) {
    ResultPair rp = result.at(i);
    printf("%3d : %-30s %lf\n", i, rp.second.c_str(), rp.first);
  }
}

SvmWrapper::SvmWrapper(int argc, char**argv, bool auto_initialize) {
  init_svm_parameter();

  if (auto_initialize) {
    if (argc == 1) 
      init_svm_problem();
    else 
      init_svm_problem_dynamic(argv[1]);
  }
  
  // LIBSVMの計算出力をオフ
  svm_set_print_string_function(&print_null);
}

SvmWrapper::~SvmWrapper() {
  dispose_svm();
}

void SvmWrapper::run() {
  get_model();
}

void SvmWrapper::dispose_svm() {
  if (problem.y)
    delete[] problem.y;
  
  if (problem.x) {
    for (int i = 0; i < problem.l; ++i)
      if (problem.x[i])
        delete[] problem.x[i];
    delete[] problem.x;
  }
}

void SvmWrapper::for_presentation(const std::vector<double>& w) {
  // include circle and rectangle
  const std::string test_file("easy_test/circle_and_rectangle.jpg");
  
  Hog test_feat(test_file.c_str(), 5, 3, 40, 40, 9);
  std::vector<double> before_w(kTotalDim), after_w(kTotalDim);
  for (unsigned i = 0; i < kTotalDim; ++i) {
    before_w[i] = after_w[i] = test_feat[i];
    after_w[i] *= w[i];
  }

  cv::namedWindow("before", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  cv::namedWindow("after", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  cv::imshow("before", VisualizeHog::generate_mat(before_w));
  cv::imshow("after", VisualizeHog::generate_mat(after_w));
  cv::waitKey(0);
  
  // std::shared_ptr<double> feat(Hog::get_feature(test_file));
  // cv::Mat before = VisualizeHog::generate_mat(feat.get());
  // for (unsigned i = 0; i < kTotalDim; ++i)
  //   feat.get()[i] *= w.at(i);
  // cv::Mat after = VisualizeHog::generate_mat(feat.get());
  // cv::namedWindow("before", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  // cv::namedWindow("after", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  // cv::imshow("before", before);
  // cv::imshow("after", after);
  

  // // each other rectangle and circle
  // const std::string rectangle("easy_test/rectangle_0002.pgm");
  // const std::string circle("easy_test/circle_1.jpg");
  // std::shared_ptr<double> rectangle_feat(Hog::get_feature(rectangle));
  // std::shared_ptr<double> circle_feat(Hog::get_feature(circle));
  // cv::Mat rectangle_mat = VisualizeHog::generate_mat(rectangle_feat.get());
  // cv::Mat circle_mat = VisualizeHog::generate_mat(circle_feat.get());
  // cv::namedWindow("rectangle" , CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  // cv::namedWindow("circle", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
  // cv::imshow("rectangle", rectangle_mat);
  // cv::imshow("circle", circle_mat);
  
  // cv::waitKey(0);
}

 
void SvmWrapper::get_model() {
  if (svm_check_parameter(&problem, &param) != NULL)
    assert(false);

  svm_model *model = svm_train(&problem, &param);
 
  std::vector<double> w(kTotalDim, 1e-10);
  for (int h = 0; h < model->l; ++h) 
    for (int i = 0; i < kTotalDim; ++i) 
      w[i] += model->SV[h][i].value * model->sv_coef[0][h];

  eval_vector(w, model->rho[0]);

  // for (int i = 0; i < model->l; ++i) {
  //   for (int j = 0; j < kTotalDim; ++j) {
  //     printf("sv %d : %d : %lf\n", i, j, model->SV[i][j].value);
  //   }
  // }

  {
    // メンバ関数に移す必要あり
    // サポートベクトルの出力
    std::ofstream sv("sv");
    // サポートベクトルの数と次元
    sv << model->l << " " << kTotalDim << std::endl;
    for (int i = 0; i < model->l; ++i) 
      for (int j = 0; j < kTotalDim; ++j) 
        sv << model->SV[i][j].value << std::endl;
  }

  // 標準出力へ表示
  // for (int i = 0; i < model->l; ++i) {
  //   printf("sv_coef : %d : is %lf\n", i, model->sv_coef[0][i]);
  // }
  printf("rho : %lf\n", model->rho[0]);

  // cv::imshowを利用して画像を表示
  if (false)
    for_presentation(w);

  // libSVMのインスタンスを開放
  svm_free_and_destroy_model(&model);
}
