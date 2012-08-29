#include "base_method.h"
#include "hog.h"
#include "random_function.h"
<<<<<<< HEAD
#include "constant.h"
=======
>>>>>>> origin/master
#include "util.h"
#include "timer.h"
#include "search_database.h"

namespace {
<<<<<<< HEAD

const int kL = 9998 * 11 + 2;

void register_svm(SvmWrapper& svm, const Hog& h, int idx, int value) {
  svm.problem.x[idx] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    svm.problem.x[idx][i].index = i + 1;
    svm.problem.x[idx][i].value = h[i] * value;
  }
  svm.problem.x[idx][kTotalDim].index = -1;
  svm.problem.y[idx] = value;
}

=======
const int kL = 11;
>>>>>>> origin/master
};

void BaseMethod::init_svm_problem() {
  // problem の初期設定
  svm.problem.l = kL;
  svm.problem.y = new double[kL];
  svm.problem.x = new svm_node * [kL];

  // ポジティブの設定
  set_positive_svm();

  // ネガティブの設定
  set_negative_svm();
}

void BaseMethod::set_positive_svm() {
<<<<<<< HEAD
  // 元画像
  Hog h0(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
  ::register_svm(svm, h0, 0, 1);

  // 回転させた画像
  Hog h1(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin, 1);
  ::register_svm(svm, h1, 1, 1);
=======
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  svm.problem.x[0] = new svm_node[kTotalDim + 1];
  for (int i = 0; i < kTotalDim; ++i) {
    svm.problem.x[0][i].index = i + 1;
    svm.problem.x[0][i].value = h[i];
  }
  svm.problem.x[0][kTotalDim].index = -1;
  svm.problem.y[0] = 1;
>>>>>>> origin/master
}

void BaseMethod::set_negative_svm() {
  std::vector<std::string> files;
<<<<<<< HEAD
  Util::get_file_list(kImageDir + "/negativeImages", files, true);

  int idx = 2;
  for (unsigned i = 0; i < files.size(); ++i) {
    // 元画像を svm に登録
    Hog h(files[i].c_str(), kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
    ::register_svm(svm, h, idx++, -1);
    // 部分画像 を svm に登録
    // 分割は 10 cell で、縦2 横5
    cv::Mat base = cv::imread(files[i].c_str(), 0);
    const int vertical_unit = base.rows / 2;
    const int horizon_unit = base.cols / 5;
    for (int j = 0; j < 5; ++j) {
      for (int k = 0; k < 2; ++k) {
        cv::Mat sub = cv::Mat(base, cv::Rect(j * horizon_unit,
                                             k * vertical_unit,
                                             horizon_unit,
                                             vertical_unit));
        cv::resize(sub, sub, cv::Size(kResizeX, kResizeY));
        Hog t(sub, kCellX, kBlockX, kResizeX, kResizeY, kCellBin);
        ::register_svm(svm, t, idx++, -1);
      }
    }
=======
  Util::get_file_list(kFeatureVectorDir, files, true);
  std::random_shuffle(files.begin(), files.end(), RandomFunction());

  for (int i = 1; i < kL; ++i) {
    std::vector<double> vd;
    Util::read_vector_data(files[i-1], vd);
    // svm へ登録
    svm.problem.x[i] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[i][j].index = j + 1;
      svm.problem.x[i][j].value = -vd[j];
    }
    svm.problem.x[i][kTotalDim].index = -1;
    svm.problem.y[i] = -1;
>>>>>>> origin/master
  }
}

void BaseMethod::run() {
  Timer timer("base method");
  this->init_svm_problem();
  wq = svm.get_weight_vector();
  SearchDatabase::search(wq);
}

void BaseMethod::run(std::vector<std::string>& ranking) {
<<<<<<< HEAD
  Timer timer("base");
=======
  Timer timer("base method");
>>>>>>> origin/master
  this->init_svm_problem();
  wq = svm.get_weight_vector();
  SearchDatabase::search(wq, ranking, 10);
}
