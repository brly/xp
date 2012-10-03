#include "linear_combination_method.h"
#include "search_database.h"
#include "hog.h"
#include "timer.h"

#include <algorithm>
#include <numeric>

void LinearCombinationMethod::init_weight_vector() {
  typedef std::vector<double> VectorDouble;
  typedef std::vector<VectorDouble> MatDouble;

  // example query
  MatDouble ws =
      SimpleRepresentVectorFactory::create_represent_vector(query_, k_);
  Hog q(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);

  // wqを線型結合で求める
  std::fill(wq.begin(), wq.end(), 0);
  const int N = static_cast<int>(ws.size());
  for (int i = 0; i < N; ++i) {
    double a = 0;
    for (int j = 0; j < kTotalDim; ++j) a += ws[i][j] * q[j];
    for (int j = 0; j < kTotalDim; ++j) wq[j] += a * ws[i][j];
  }
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
    Timer timer("linear");
    init_weight_vector();
  }

  // SearchDatabase::search(wq, ranking, 10);
  SearchDatabase::search(wq, ranking, 0);
}
