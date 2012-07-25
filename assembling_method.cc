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
  svm.problem.l = kM;
  svm.problem.x = new svm_node * [kM + 1];
  svm.problem.y = new double[kM + 1];

  // ポジティブの設定
  // TODO: 適当に関数化. あとで修正する
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
  std::string query = "easy_test/circle_0.jpg";
  Hog h(query.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
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
  std::string query = "circle_0";
  std::vector<std::string> samples;

  // ファイルリストを取得
  Util::get_file_list(kFeatureVectorDir, samples);

  // 先頭の kGroupM 個を学習サンプルに使用
  std::random_shuffle(samples.begin(), samples.end(), RandomFunction());

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
  std::string query = "circle_0.w";
  std::vector<std::string> samples;
  // ファイル集合を取得
  Util::get_file_list(kWeightVectorDir, samples);

  // クエリが存在する場合は除去
  // その他、そぐわないものについはこの時点で除去すべきである
  samples.erase(std::remove(samples.begin(), samples.end(), query), samples.end());

  // シャッフルを行い、先頭から kGroupM 個のものについて代表と定める
  std::random_shuffle(samples.begin(), samples.end(), RandomFunction());

  // 代表の重みベクトルを求める
  typedef std::vector<double> Vec;
  std::vector<Vec> ws;
  for (int i = 0; i < kGroupM; ++i) {
    Vec t;
    Util::read_vector_data(samples[i], t);
    ws.push_back(t);
  }
  
  // 先頭のGroupM個について、クエリとの類似度の指標 a_i を求める
  std::vector<double> alphas;
  for (int i = 0; i < kGroupM; ++i) {
    double alpha = 0;
    for (int j = 0; j < kTotalDim; ++j) alpha += ws[i][j] * q[j];
    alphas.push_back(alpha);
  }

  // DBを読み込む
  // この処理は常に決定的なのでどこか別で初期化時などにしたほうがいい
  std::vector<Vec> db;
  std::vector<std::string> db_str;
  Util::get_file_list(kFeatureVectorDir, db_str);
  for (unsigned i = 0; i < db_str.size(); ++i) {
    Vec t;
    Util::read_vector_data(db_str[i], t);
    db.push_back(t);
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

    // DBから ws[select_idx] に近い画像を集める
    typedef std::pair<double, std::string> P;
    std::priority_queue<P, std::vector<P>, std::greater<P> > pq;

    for (int j = 0; j < db.size(); ++j) {
      double d = Util::get_vector_dot(ws[select_idx], db[j]);
      if (pq.size() < 10) {
        pq.push(P(d, db_str[j]));
      } else if (pq.top().first < d) {
        pq.pop();
        pq.push(P(d, db_str[j]));
      }
    }

    while (!pq.empty()) {
      P p = pq.top(); pq.pop();
      printf("%.6f : %s\n", p.first, p.second.c_str());
    }
    
    scanf("%lf", &t);
  }
}

void AssemblingMethod::run() {
  Timer timer("assembling method");
  this->init_svm_problem();
  SearchDatabase::search(wq, 10);
}
