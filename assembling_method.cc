#include "assembling_method.h"
#include "random_function.h"
#include "hog.h"
#include "constant.h"
#include "util.h"
#include "search_database.h"
#include "timer.h"

#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <cassert>
#include <numeric>
#include <utility>
#include <ctime>
#include <queue>

#include <dirent.h>

void AssemblingMethod::init_svm_problem() {
  if (kBeta >= 100 || kBeta < 0) {
    assert(false);
  }

  // svm の初期設定
  svm.problem.l = kM+1;
  svm.problem.x = new svm_node * [kM+1];
  svm.problem.y = new double[kM+1];

  // ポジティブの設定
  set_positive_svm();

  // ネガティブの設定
  // それぞれのグループのサンプル数
  const int kGroupA_M = kM * (kBeta / 100.0);
  const int kGroupB_M = kM - kGroupA_M;
  int idx = 1;
  
  // A. ランダムにDBから選択
  set_negative_groupA_svm(kGroupA_M, idx);
  
  // B. 代表となる画像から選択
  set_negative_groupB_svm(kGroupB_M, idx);

  // SVM で重みベクトルを計算
  wq = svm.get_weight_vector();
}

////////////////////////////////////////////////////////////////////////////////
// ポジティブの設定を行う
// 外部から変更できるようにのちのち変更する
void AssemblingMethod::set_positive_svm() {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
  svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    svm.problem.x[0][i].index = i + 1;
    svm.problem.x[0][i].value = h[i];
    // ここで内部保持するクエリのベクトルも初期化 .. ひどい
    q[i] = h[i];
  }
  svm.problem.x[0][kTotalDim].index = -1;
  svm.problem.y[0] = 1;
}

void AssemblingMethod::set_negative_groupA_svm(const int kGroupM, int& idx) {
  std::vector<std::string> samples;

  // ファイルリストを取得
  Util::get_file_list(kFeatureVectorDir, samples, true);

  // 先頭の kGroupM 個を学習サンプルに使用
  RandomFunction r;
  std::random_shuffle(samples.begin(), samples.end(), r);

  for (int i = 0; i < kGroupM; ++i) {
    std::vector<double> vd;
    Util::read_vector_data(samples[i], vd);

    // svm へ設定
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

void AssemblingMethod::set_negative_groupB_svm(const int kGroupM, int& idx) {
  typedef std::vector<double> VectorDouble;
  typedef std::vector<VectorDouble> MatDouble;
  
  // ファイル集合を取得
  std::vector<std::string> samples;
  Util::get_file_list(kWeightVectorDir , samples, true);

  // シャッフルを行い、先頭から kGroupM 個のものについて代表と定める
  RandomFunction r;
  std::random_shuffle(samples.begin(), samples.end(), r);

  // 代表の重みベクトルを求める
  std::vector<VectorDouble> ws(kGroupM);
  for (int i = 0; i < kGroupM; ++i) {
    Util::read_vector_data(samples[i], ws[i]);
  }
  
  // 先頭のGroupM個について、クエリとの類似度の指標 a_i を求める
  std::vector<double> alphas;
  for (int i = 0; i < kGroupM; ++i) {
    double alpha = 0;
    for (int j = 0; j < kTotalDim; ++j) alpha += ws[i][j] * q[j];
    alphas.push_back(alpha);
  }

  // 各代表の近傍画像リストより、ベクトルを取得する
  std::vector<MatDouble> nearest_image_vectors;
  for (int i = 0; i < kGroupM; ++i) {
    MatDouble m;
    
    // 読み込み先ファイル名の作成
    std::string source = samples[i];
    for (int j = 0; j < 2; ++j)
      source = source.substr(source.find('/') + 1);
    source = kNearestImageDir + "/" + source;

    // 一行ずつ読み込む
    std::ifstream ifs(source);
    std::string line;
    while (std::getline(ifs, line)) {
      VectorDouble t;
      if (!Util::is_exist(line.c_str())) continue;
      Util::read_vector_data(line, t);
      m.push_back(t);
    }

    nearest_image_vectors.push_back(m);
  }

  // kGroupM 個のサンプルを追加する
  const double sum_alpha = std::accumulate(alphas.begin(), alphas.end(), 0.0);
  for (int i = 0; i < kGroupM; ) {
    // ランダムに選択する
    double r = static_cast<double>(rand()) / RAND_MAX, t = 0;
    int select_idx = -1;
    for (unsigned j = 0; j < alphas.size() - 1; ++j) {
      double p = alphas[j] / sum_alpha;
      if (t + p > r) {
        select_idx = j;
        break;
      }
      t += p;
    }
    if (select_idx == -1) select_idx = alphas.size() - 1;

    for (int j = 0; j < 3 && i < kGroupM; ++j) {
      const VectorDouble& data_ref = nearest_image_vectors[select_idx][j];
      svm.problem.x[idx] = new svm_node[kTotalDim + 1];
      for (int k = 0; k < kTotalDim; ++k) {
        svm.problem.x[idx][k].index = k + 1;
        svm.problem.x[idx][k].value = -data_ref[k];
      }
      svm.problem.x[idx][kTotalDim].index = -1;
      svm.problem.y[idx] = -1;
      ++i; ++idx;
    }
    
    // previous method
    
    // // 自身を加える
    // if (i < kGroupM) {
    //   svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    //   for (int j = 0; j < kTotalDim; ++j) {
    //     svm.problem.x[idx][j].index = j + 1;
    //     svm.problem.x[idx][j].value = -ws[select_idx][j];
    //   }
    //   svm.problem.x[idx][kTotalDim].index = -1;
    //   svm.problem.y[idx] = -1;
    //   ++i; ++idx;
    // }

    // // 近いものを加える
    // while (!pq.empty() && i < kGroupM) {
    //   P p = pq.top(); pq.pop();
    //   const int v = p.second;
    //   svm.problem.x[idx] = new svm_node[kTotalDim + 1];
    //   for (int j = 0; j < kTotalDim; ++j) {
    //     svm.problem.x[idx][j].index = j + 1;
    //     svm.problem.x[idx][j].value = -db[v][j];
    //   }
    //   svm.problem.x[idx][kTotalDim].index = -1;
    //   svm.problem.y[idx] = -1;
    //   ++i; ++idx;
    // }
  }
}

void AssemblingMethod::run() {
  {
    Timer timer("assembling method");
    this->init_svm_problem();
  }
  SearchDatabase::search(wq);
}

void AssemblingMethod::run(std::vector<std::string>& ranking) {
  {
    Timer timer("assembling");
    this->init_svm_problem();
  }

  // SearchDatabase::search(wq, ranking, 10);
  SearchDatabase::search(wq, ranking, 0);
}
