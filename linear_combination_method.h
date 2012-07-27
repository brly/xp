#ifndef LINEAR_COMBINATION_METHOD_H_
#define LINEAR_COMBINATION_METHOD_H_

// この手法では、事前に計算された重みベクトル (SVMにより導出されたベクトル) を
// 利用するため、手法内ではLIBSVMを用いることはない

// 代表となる画像を複数枚、ランダムに選択し、
// クエリ画像のベクトル表現 (q)と、各代表の重みベクトル (w_i) の
// 内積 (q^T w_i = a_i) を求め、各代表の重みベクトルを a_i で重み付けをし、
// それらを線型結合したものをクエリ画像の重みベクトルとする手法

#include "method.h"
#include "constant.h"
#include "simple_represent_vector_factory.h"

#include <vector>
#include <string>
#include <utility>

class LinearCombinationMethod : public Method {
  std::vector<double> wq;
  
  void init_weight_vector();

  void scanning_database_by_wq();
  
 public:
  explicit LinearCombinationMethod() : wq(kTotalDim, 0) {}
  
  void run();
};

#endif // LINEAR_COMBINATION_METHOD_H_
