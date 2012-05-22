// LIBSVMとの仲立ちを行う
// 達成する事
// 1. svm_train()を呼び出すための事前設定
//    事前設定(param, problem)には、画像ファイルの情報が必要
// 2. svm_train()を呼ぶ

#ifndef SVM_WRAPPER_H
#define SVM_WRAPPER_H

#include "svm.h"

// インターフェース

void init_svm();

void dispose_svm();

void get_model();

#endif // SVM_WRAPPER_H
