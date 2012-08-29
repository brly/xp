#ifndef C_METHOD_H_
#define C_METHOD_H_

// 手法 C
// 通常の手法と同様に SVM を利用して求めるが、
// ネガティブなクラスへ登録するデータとして
// 内積とランダムに選択された画像を用いる

// ネガティブなクラスへ設定するデータ数を kM, 
// ランダムに選択されるデータの割合を KBeta percent とする。

#include "method.h"
#include "svm_wrapper.h"
#include "constant.h"

class CMethod : public Method {
  SvmWrapper svm;
  std::vector<double> wq, q;
  const int kM, kBeta;
  std::string query_;

  void init_svm_problem();

  void set_positive_problem();

  void set_negative_problem_random(const int kRandom, int& idx);

  void set_negative_problem_near(const int kBeta, int& idx);
  
 public:
  explicit CMethod(const std::string& query, int m, int beta)
      : svm(1, NULL, false), wq(kTotalDim, 0), q(kTotalDim, 0), kM(m),
        kBeta(beta), query_(query) {}

  void run();

  void run(std::vector<std::string>& ranking);
};

#endif // C_METHOD_H_

