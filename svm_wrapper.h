// LIBSVMとの仲立ちを行う
// 達成する事
// 1. svm_train()を呼び出すための事前設定
//    事前設定(param, problem)には、画像ファイルの情報が必要
// 2. svm_train()を呼ぶ

#ifndef SVM_WRAPPER_H
#define SVM_WRAPPER_H

#include "svm.h"

#include <vector>
#include <cstdlib>

class SimpleMethod;

class SvmWrapper {
  // friend宣言
  friend class SimpleMethod;
  
  // LibSVMに必要なパラメータインスタンス
  svm_parameter param;
  svm_problem problem;

  // paramの初期化を行う
  void init_svm_parameter();

  // problemの初期化を行う(legacy code)
  void init_svm_problem();
  void init_svm_problem_dynamic(const char* name);

  // データベースと比較し、ランキングをcui表示する
  void eval_vector(const std::vector<double>& w, const double rho);

  // libSVMのAPIを利用して、線形SVMの情報を得る
  // TODO: 関数名変更
  void get_model();
  
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
