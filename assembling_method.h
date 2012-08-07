#ifndef ASSEMBLING_METHOD_H_
#define ASSEMBLING_METHOD_H_

// この手法では、SVMの学習用サンプル数を減らすことで計算速度の向上について
// 考えるため、LIBSVMは内部で使用される
// 
// 使用される学習用サンプルとして、以下の 2 グループに分類される
// 
// A. ランダムにDBから選択
// B. 代表となる画像から選択
//
// グループ B については、予め 重みベクトル w_i がわかっているものとし、
// w_iとクエリのベクトル表現との内積 の計算結果の値が、
// 学習用サンプルとして選択される確率となる

// TODO:
// -クエリ入力部分を書く
// -SVMへ学習サンプルを設定する部分を関数化する
// -readdir 部分は、std::vector<std::string> を返す、もしくは
// 引数に参照を取るようにするほうが簡潔にかけて良い気がする

#include "method.h"
#include "svm_wrapper.h"
#include "hog.h"
#include "constant.h"

#include <vector>
#include <string>

class AssemblingMethod : public Method {
  SvmWrapper svm;
  std::vector<double> wq, q;
  const int kM, kBeta;
  std::string query_;

  void init_svm_problem();

  void set_positive_svm();

  void set_negative_groupA_svm(const int kGroupM, int& idx);

  void set_negative_groupB_svm(const int kGroupM, int& idx);
  
 public:
  explicit AssemblingMethod(const std::string& query, int m, int beta)
      : svm(1, NULL, false), wq(kTotalDim, 0), q(kTotalDim), kM(m), kBeta(beta),
        query_(query){}
  
  void run();
  void run(std::vector<std::string>& ranking);
};

#endif // ASSEMBLING_METHOD_H_
