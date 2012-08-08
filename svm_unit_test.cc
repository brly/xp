
#include "constant.h"
#include "svm_wrapper.h"
#include "util.h"
#include "hog.h"
#include "random_function.h"

#include <opencv2/opencv.hpp>

#include <cstdio>
#include <vector>
#include <string>
#include <algorithm>

void register_svm_pos(SvmWrapper& svm, const Hog& h, const int idx) {
  svm.problem.x[idx] = new svm_node[kTotalDim + 1];
  for (int j = 0; j < kTotalDim; ++j) {
    svm.problem.x[idx][j].index = j + 1;
    svm.problem.x[idx][j].value = h[j];
  }
  svm.problem.x[idx][kTotalDim].index = -1;
  svm.problem.y[idx] = 1;
}

void register_svm_neg(SvmWrapper& svm, const Hog& h, const int idx) {
  svm.problem.x[idx] = new svm_node[kTotalDim + 1];
  for (int j = 0; j < kTotalDim; ++j) {
    svm.problem.x[idx][j].index = j + 1;
    svm.problem.x[idx][j].value = -h[j];
  }
  svm.problem.x[idx][kTotalDim].index = -1;
  svm.problem.y[idx] = -1;
}

int main() {
  std::vector<std::string> files;
  Util::get_file_list(kImageDir + "/negativeImages", files, true);
  
  const std::string query = "easy_test/circle_0.jpg";

  typedef std::vector<double> Vec;
  std::vector<Vec> feature_vector;

  clock_t st = clock();

  // クエリのベクトルを取得
  Hog h0(query.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
  Hog h1(query.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin, 1);

  // svm のデータ数を設定
  const int K = 9998 * 11 + 2;

  // Wq 保存先
  std::vector<double> wq;
  
  // とりあえず繰り返し無し
  for (int z = 0; z < 1; ++z) {
    printf("%d iteration. ", z+1);
    
    // svm 初期設定
    SvmWrapper svm(1, NULL, false);
    svm.problem.l = K;
    svm.problem.y = new double[K];
    svm.problem.x = new svm_node * [K];

    // ポジティブクラスの設定
    register_svm_pos(svm, h0, 0);
    register_svm_pos(svm, h1, 1);

    int idx = 2;
    for (unsigned i = 0; i < files.size(); ++i) {
      // 元画像の特徴ベクトルを取得
      Hog org(files[i].c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
      // 元画像を svm に登録
      register_svm_neg(svm, org, idx++);

      // 部分画像の作成
      // 5 x 2 の窓に分割することを想定
      cv::Mat base = cv::imread(files[i].c_str(), 0);
      const int vertical_unit = base.rows / 2;
      const int horizon_unit = base.cols / 5;
      for (int j = 0; j < 5; ++j) {
        for (int k = 0; k < 2; ++k) {
          cv::Mat sub = cv::Mat(base,
                                cv::Rect(j * (horizon_unit), k * (vertical_unit),
                                         horizon_unit, vertical_unit));
          cv::resize(sub, sub, cv::Size(kResizeX, kResizeY));
          Hog h(sub, kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
          register_svm_neg(svm, h, idx++);
        }
      }
      printf("%s : %d / %d\n", files[i].c_str(), idx, K);
    }
    
    wq = svm.get_weight_vector();

    printf("done.\n");
  }

  printf("%f\n", ((double)(clock() - st)) / CLOCKS_PER_SEC);

  return 0;
}
