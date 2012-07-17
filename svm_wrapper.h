#ifndef SVM_WRAPPER_H
#define SVM_WRAPPER_H

// LIBSVM との仲立ちを行う
// 達成する事
// 1. svm_train ()を呼び出すための事前設定
//    事前設定(param, problem)には、画像ファイルの情報が必要
// 2. svm_train ()を呼ぶ

#include "svm.h"

#include <vector>
#include <cstdlib>

class SimpleMethod;

class SvmWrapper {
  // friend 宣言
  friend class SimpleMethod;
  friend class AssemblingMethod;
  
  // LibSVM に必要なパラメータインスタンス
  svm_parameter param;
  svm_problem problem;

  // param の初期化を行う
  void init_svm_parameter();

  // problem の初期化を行う(legacy code)
  void init_svm_problem();
  void init_svm_problem_positive_default(int& idx);
  void init_svm_problem_negative_default(int& idx);
  
  void init_svm_problem_dynamic(const char* name);

  // データベースと比較し、ランキングをcui表示する
  void eval_vector(const std::vector<double>& w, const double rho);

  // libSVM の API を利用して、線形SVMの情報を得る
  // TODO: 関数名変更
  void get_model();

  // get_model() と異なり、重みベクトルが関数の返り値であり、
  // また eval_vector() が内部で呼び出されないため、ランキングとも比較されない
  // get_model() または get_weight_vector() のどちらか一方が一度が呼び出されると
  // param, problem メンバは破棄される
  std::vector<double> get_weight_vector();
  
  // problemを破棄する
  void dispose_svm();

  // 画像の特徴量をguiで表現する
  void for_presentation(const std::vector<double>& w);
 public:
  // コンストラクタ
  // 各引数はSvmWrapperの初期化方法を表現している
  SvmWrapper(int argc = 1, char**argv = NULL, bool auto_initialize = true);
  ~SvmWrapper();
  void run();
};

#endif // SVM_WRAPPER_H
