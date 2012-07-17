#include "assembling_method.h"
#include "random_function.h"
#include "hog.h"
#include "constant.h"

#include <fstream>
#include <cstdlib>

void AssemblingMethod::init_svm_problem() {
  // 重みベクトルを導出する代表の数
  // const int kSampleL = 30 + 1;
  // svm.problem.l = kSampleL;
  // svm.problem.y = new double[kSampleL];
  // svm.problem.x = new svm_node * [kSampleL];

  // 代表を選択
  std::vector<std::string> represents;
  set_represents(represents, "NEGATIVE_sample");

  Hog q("easy_test/circle_0.jpg", kCellX, kBlockX, kResizeX, kResizeY,
        kOrientation);

  // 各代表について重みベクトルを求める
  for (unsigned i = 0; i < represents.size(); ++i) {
    // 初期化
    SvmWrapper temp_svm(1, NULL, false);
    get_svm_each_represent(temp_svm, represents[i]);
    
    std::vector<double> w = temp_svm.get_weight_vector();
    double s = 0;
    for (unsigned j = 0; j < kTotalDim; ++j)
      s += q[j] * w[j];
    printf("%s : %f\n", represents[i].c_str(), s);
  }
}

void AssemblingMethod::set_represents(std::vector<std::string>& represents,
                                      const char* file_name) {
  std::ifstream set_stream(file_name);
  std::string line;
  while (std::getline(set_stream, line))
    represents.push_back(line);
  std::random_shuffle(represents.begin(), represents.end(), RandomFunction());
  represents.erase(represents.begin() + 1, represents.end());
}

void AssemblingMethod::get_svm_each_represent(SvmWrapper& temp_svm,
                                              const std::string& represent) {
  Hog repre(represent.c_str(), kCellX, kBlockX, kResizeX, kResizeY,
            kOrientation);      
  const int kInnerSampleL = 10000; // Hard Coding ..
  temp_svm.problem.l = kInnerSampleL;
  temp_svm.problem.y = new double[kInnerSampleL];
  temp_svm.problem.x = new svm_node *[kInnerSampleL];
  // represents[i] を ポジティブで設定
  temp_svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    temp_svm.problem.x[0][i].index = i + 1;
    temp_svm.problem.x[0][i].value = repre[i];
  }
  temp_svm.problem.x[0][kTotalDim].index = -1;
  temp_svm.problem.y[0] = 1;
  // ネガティブ設定
  std::string line;
  std::ifstream negative_set_stream("NEGATIVE_sample"); // Hard Coding ..
  int idx = 1;
  while (std::getline(negative_set_stream, line)) {
    if (represent == line) continue;
    Hog hog(line.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    temp_svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      temp_svm.problem.x[idx][i].index = i + 1;
      temp_svm.problem.x[idx][i].value = -hog[i];
    }
    temp_svm.problem.x[idx][kTotalDim].index = -1;
    temp_svm.problem.y[idx] = -1;
    ++idx;
  }
}

void AssemblingMethod::run() {
  this->init_svm_problem();
  // svm.run();
}
