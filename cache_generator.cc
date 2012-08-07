
// キャッシュ可能なデータをのキャッシュを作成するプログラム
// 該当するデータ
// - 画像の特徴ベクトル
// - 画像の重みベクトル

#include "hog.h"
#include "constant.h"
#include "svm_wrapper.h"
#include "util.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <vector>
#include <algorithm>
#include <sys/stat.h>
#include <dirent.h>

namespace {

const int kUserPermission[3] = { S_IRUSR, S_IWUSR, S_IXUSR };
const int kGroupPermission[3] = { S_IRGRP, S_IWGRP, S_IXGRP };
const int kOtherPermission[3] = { S_IROTH, S_IWOTH, S_IXOTH };

};

// 前方宣言

class CacheGenerator {
  static int directory_check();
  static int get_permission(int permission);
  static bool is_exist(const char* path);
  static int make_feature_vector_file();
  static int make_weight_vector_file();
  static void wrap_chdir(const char* path);
 public:
  static void run();
};

////////////////////////////////////////////////////////////////////////////////
// run
void CacheGenerator::run() {
  // ディレクトリの存在を確認し、存在しなければ作成
  if (directory_check() != 0) {
    puts("ディレクトリ操作で失敗.");
    return;
  }

  // 特徴ベクトルを記述したファイルを作成
  if (make_feature_vector_file() != 0) {
    puts("特徴ベクトルが記述されたファイルを作成中にエラー.");
    return;
  }

  return;
  
  // 画像に依存している重みベクトルを作成
  if (make_weight_vector_file() != 0) {
    puts("画像に依存している重みベクトルを作成中にエラー");
    return;
  }

  // 正常に終了
  puts("正常に終了しました.");
}

////////////////////////////////////////////////////////////////////////////////
// 処理に必要なディレクトリが存在するか確認する
// 確認後、正常な状態であれば 0 , そうでない場合は 1 が返る
int CacheGenerator::directory_check() {
  // ./cache/ が存在するか
  if (!is_exist(kCacheDir.c_str())) {
    int ret = mkdir(kCacheDir.c_str(), get_permission(775));
    // mkdir "cache" が失敗した場合
    if (ret != 0) {
      printf("mkdir %s に失敗.\n", kCacheDir.c_str());
      return 1;
    }    
  }

  // ./cache/feature_vector/ が存在するか
  if (!is_exist(kFeatureVectorDir.c_str())) {
    int ret = mkdir(kFeatureVectorDir.c_str(), get_permission(775));
    // mkdir "cache/feature_vector" が失敗した場合
    if (ret != 0) {
      printf("mkdir %s に失敗.\n", kFeatureVectorDir.c_str());
      return 1;
    }    
  }

  // ./$kImageDir/ が存在するか
  if (!is_exist(kImageDir.c_str())) {
    printf("画像ファイルを格納しているディレクトリ : %s が見つかりません.\n",
           kImageDir.c_str());
    return 1;
  }

  // ./$kWeightVectorDir/ が存在するか
  if (!is_exist(kWeightVectorDir.c_str())) {
    int ret = mkdir(kWeightVectorDir.c_str(), get_permission(775));
    // mkdir "$kWeightVectorDir" が失敗した場合
    if (ret != 0) {
      printf("mkdir %s に失敗.\n", kWeightVectorDir.c_str());
      return 1;
    }
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 10進数のパーミッションを引数にとり、(例:755, 700 ..)
// 実際のパーミッションに設定されるintの値を返す
int CacheGenerator::get_permission(int permission) {
  int ret = 0;

  // 所有者
  int n = permission / 100;
  for (int num = 4, idx = 0; num >= 1; num /= 2, ++idx) {
    if (n >= num) {
      ret |= kUserPermission[idx];
      n -= num;
    }
  }

  // グループ
  n = permission % 100 / 10;
  for (int num = 4, idx = 0; num >= 1; num /= 2, ++idx) {
    if (n >= num) {
      ret |= kGroupPermission[idx];
      n -= num;
    }
  }

  // 他者
  n = permission % 10;
  for (int num = 4, idx = 0; num >= 1; num /= 2, ++idx) {
    if (n >= num) {
      ret |= kOtherPermission[idx];
      n -= num;
    }
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
// stat 関数を用いて、引数のパスが存在するか確認する関数
// 存在する場合は true, そうでない場合は false を返す
bool CacheGenerator::is_exist(const char* file) {
  // stat 関数用オブジェクト
  struct stat stat_buf;
  return stat(file, &stat_buf) == 0;
}

////////////////////////////////////////////////////////////////////////////////
// ./$kImageDir/ を出力先として、画像の特徴ベクトルのデータを作成する関数
// 内部では chdir 関数が呼ばれているが、関数終了後は、カレントに存在する
int CacheGenerator::make_feature_vector_file() {
  std::vector<std::string> files;
  Util::get_file_list(kImageDir, files, true);
  for (unsigned i = 0; i < files.size(); ++i) {
    // 画像ファイルでない場合は飛ばす
    if (files[i].find(".pgm") == -1 && files[i].find(".jpg") == -1) continue;

    // キャッシュ作成先のパスを作成
    std::string destination = kFeatureVectorDir +
        files[i].substr(files[i].find("/"));
    destination.erase(destination.size() - 4);

    // キャッシュが存在する場合は飛ばす
    if (is_exist(destination.c_str())) continue;

    // キャッシュ作成先のディレクトリに問題がないか確認
    Util::mkdir_rec(destination);
    
    Hog hog(files[i].c_str(), kCellX, kBlockX, kResizeX, kResizeY,
            kOrientation);
    Util::write_vector_data(destination, hog.get_data());

    printf("%s is created.\n", destination.c_str());
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 画像に依存している重みベクトルを作成する関数
int CacheGenerator::make_weight_vector_file() {
  std::vector<std::string> files;
  // Util::get_file_list(kFeatureVectorDir, files, true);
  Util::get_file_list(kFeatureVectorDir + "/caltech101", files, true);
  
  typedef std::vector<double> Vec;
  std::vector<Vec> feature_vector;

  for (unsigned i = 0; i < files.size(); ++i) {
    Vec t;
    Util::read_vector_data(files[i], t);
    feature_vector.push_back(t);
  }

  // ランダム選択用の配列を作成し初期化
  std::vector<int> indice(files.size(), 0);
  for (int i = 0; i < indice.size(); ++i) indice[i] = i;

  for (unsigned i = 0; i < files.size(); ++i) {
    // キャッシュ作成先パスを作成
    std::string destination = files[i];
    for (int j = 0; j < 2; ++j) 
      destination = destination.substr(destination.find("/") + 1);
    destination = kWeightVectorDir + "/" + destination + ".w";

    // キャッシュ先ディレクトリ確認
    Util::mkdir_rec(destination);
    
    // キャッシュ作成先のパスが存在する場合は飛ばす    
    if (is_exist(destination.c_str())) continue;
    printf("%s\n", destination.c_str());
    
    // ランダムに混ぜる
    std::random_shuffle(indice.begin(), indice.end());

    // 先頭から kSize だけをサンプルとして扱う
    const int kSize = 10000;

    // svm の初期設定
    const int K = 1 + (10000 - 1);
    SvmWrapper svm(1, NULL, false);
    svm.problem.l = K;
    svm.problem.y = new double[K];
    svm.problem.x = new svm_node * [K];
    
    // ポジティブ(自分)の設定
    svm.problem.x[0] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[0][j].index = j + 1;
      svm.problem.x[0][j].value = feature_vector[i][j];
    }
    svm.problem.x[0][kTotalDim].index = -1;
    svm.problem.y[0] = 1;
    
    // ネガティブの設定
    for (int j = 0, idx = 1; idx < K; ++j) {
      if (indice[j] == (int)i) continue;
      svm.problem.x[idx] = new svm_node[kTotalDim + 1];
      for (int k = 0; k < kTotalDim; ++k) {
        svm.problem.x[idx][k].index = k + 1;
        svm.problem.x[idx][k].value = -feature_vector[indice[j]][k];
      }
      svm.problem.x[idx][kTotalDim].index = -1;
      svm.problem.y[idx] = -1;
      ++idx;
    }

    // ファイルへ出力
    Vec t = svm.get_weight_vector();
    Util::write_vector_data(destination, t);
  }
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// chdir 関数をひとつラップしたもの
void CacheGenerator::wrap_chdir(const char* path) {
  if (chdir(path) != 0) {
    printf("chdir(%s) に失敗\n", path);
    assert(false);
  }
}

////////////////////////////////////////////////////////////////////////////////
// main 
int main() {
  CacheGenerator::run();
  return 0;
}
