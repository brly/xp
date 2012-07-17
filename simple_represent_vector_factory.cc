#include "simple_represent_vector_factory.h"
#include "hog.h"
#include "constant.h"
#include "random_function.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>

void SimpleRepresentVectorFactory::select_represent(
    std::vector<std::string>& vs, const std::string& query) {
  // Hard Coding
  std::ifstream db_stream("NEGATIVE_sample");
  std::string line;
  while (std::getline(db_stream, line))
    vs.push_back(line);
  vs.erase(std::remove(vs.begin(), vs.end(), query), vs.end());
  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());

  // k = 5 に仮定 ( vs.size() >> 5 )
  const int k = 3;
  vs.erase(vs.begin() + k, vs.end());
  for (int i = 0; i < k; ++i) printf("%s\n", vs[i].c_str());
}

void SimpleRepresentVectorFactory::set_svm_each_represent(
    SvmWrapper& svm, const std::string& represent) {
  Hog rep(represent.c_str(),
          kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  const int K = 1 + (10000 - 1); // = 10000, Hard Coding ..
  svm.problem.l = K;
  svm.problem.y = new double[K];
  svm.problem.x = new svm_node * [K];

  // 代表をポジティブで設定
  svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < 0; ++i) {
    svm.problem.x[0][i].index = i + 1;
    svm.problem.x[0][i].value = rep[i];
  }
  svm.problem.x[0][kTotalDim].index = -1;
  svm.problem.y[0] = 1;

  // その他をネガティブで設定
  std::string line;
  std::ifstream negative_stream("NEGATIVE_sample"); // Hard Coding..
  int idx = 1;
  while (std::getline(negative_stream, line)) {
    if (line == represent) continue;
    Hog hog(line.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    for (int i = 0; i < kTotalDim; ++i) {
      svm.problem.x[idx][i].index = i + 1;
      svm.problem.x[idx][i].value = -hog[i];
    }
    svm.problem.x[idx][kTotalDim].index = -1;
    svm.problem.y[idx] = -1;
    ++idx;
  }
}

SimpleRepresentVectorFactory::Mat
SimpleRepresentVectorFactory::create_represent_vector(
    const std::string& query) {
  // 代表にする画像を決定
  std::vector<std::string> represents;
  select_represent(represents, query);

  // 代表の重みベクトルを求める
  Mat ws;
  for (unsigned i = 0; i < represents.size(); ++i) {
    SvmWrapper svm(1, NULL, false);
    set_svm_each_represent(svm, represents[i]);
    ws.push_back(svm.get_weight_vector());
    printf("%d : %s\n", i, represents[i].c_str());
  }

  return ws;
}

