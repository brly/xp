#include "linear_combination_method.h"
#include "search_database.h"
#include "hog.h"
#include "timer.h"

#include <algorithm>
#include <numeric>

void LinearCombinationMethod::init_weight_vector() {
<<<<<<< HEAD
  typedef std::vector<double> VectorDouble;
  typedef std::vector<VectorDouble> MatDouble;

  // example query
  MatDouble ws =
      SimpleRepresentVectorFactory::create_represent_vector(query_, k_);
=======
  typedef std::vector<double> Vec;
  typedef std::vector<Vec> Mat;

  // example query
  // const std::string query = kImageDir + "/circle_0.jpg";
  Mat ws = SimpleRepresentVectorFactory::create_represent_vector(query_);
>>>>>>> origin/master
  Hog q(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);

  // wqを線型結合で求める
  std::fill(wq.begin(), wq.end(), 0);
  const int N = static_cast<int>(ws.size());
  for (int i = 0; i < N; ++i) {
    double a = 0;
    for (int j = 0; j < kTotalDim; ++j) a += ws[i][j] * q[j];
<<<<<<< HEAD
    for (int j = 0; j < kTotalDim; ++j) wq[j] += a * ws[i][j];
  }
=======
    // printf("%d : %f\n", i, a);
    for (int j = 0; j < kTotalDim; ++j) wq[j] += a * q[j];
  }

  // 念の為, wq の和を表示してみる
  // printf("Wq sum is %f.\n", std::accumulate(wq.begin(), wq.end(), 0.0));
>>>>>>> origin/master
}

void LinearCombinationMethod::run() {
  {
    Timer timer("linear combination method");
    init_weight_vector();
  }
  SearchDatabase::search(wq);
}

void LinearCombinationMethod::run(std::vector<std::string>& ranking) {
  {
<<<<<<< HEAD
    Timer timer("linear");
    init_weight_vector();
  }

  // SearchDatabase::search(wq, ranking, 10);
  SearchDatabase::search(wq, ranking, 0);
=======
    Timer timer("linear combination method");
    init_weight_vector();
  }
  SearchDatabase::search(wq, ranking, 10);
>>>>>>> origin/master
}
