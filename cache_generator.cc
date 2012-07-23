
// キャッシュ可能なデータをのキャッシュを作成するプログラム
// 該当するデータ
// - 画像の特徴ベクトル
// - 画像の重みベクトル

#include "hog.h"
#include "constant.h"
#include "svm_wrapper.h"

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

const std::string kCacheDir = "cache";
const std::string kFeatureVectorDir = kCacheDir + "/feature_vector";
const std::string kWeightVectorDir = kCacheDir + "/weight_vector";
const std::string kImageDir = "easy_test";

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
  DIR *dir_ptr;
  struct stat stat_buf;
  struct dirent *entry;

  // ./$kImageDir/ のファイルオープン
  if ((dir_ptr = opendir(kImageDir.c_str())) == NULL) {
    printf("ディレクトリ : %s のオープンに失敗しました.\n",
           kImageDir.c_str());
    return 1;
  }

  // cd 
  wrap_chdir(kImageDir.c_str());

  // ファイルのグラブ
  while ((entry = readdir(dir_ptr)) != NULL) {
    lstat(entry->d_name, &stat_buf);
    
    // ファイルがディレクトリの場合は飛ばす
    if (S_ISDIR(stat_buf.st_mode)) continue;

    // パスを作成
    std::string file = entry->d_name;

    // パスが指定の拡張子を持たない場合は飛ばす
    if (file.find(".pgm") == -1 && file.find(".jpg") == -1) continue;

    // キャッシュ作成先のパスを作成
    std::string destination = std::string("../") + kFeatureVectorDir + "/" + file;
    destination.erase(destination.size() - 4);

    // キャッシュデータが既に存在する場合は飛ばす
    if (is_exist(destination.c_str())) continue;

    Hog hog(file.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);

    // キャッシュ作成先のファイルオープン
    FILE *file_ptr = nullptr;
    if ((file_ptr = fopen(destination.c_str(), "w")) == NULL) {
      printf("[%s] : のファイルオープンに失敗.\n", destination.c_str());
      printf("%s\n", strerror(errno));
      return 1;
    }

    // ファイルへ書き込み
    for (int i = 0; i < kTotalDim; ++i) {
      if (i) fputc(' ', file_ptr);
      if (fprintf(file_ptr, "%f", hog[i]) < 0) {
        printf("%s : のファイル書き込みに失敗.\n", destination.c_str());
        return 1;
      }
    }

    fclose(file_ptr);
  }

  closedir(dir_ptr);

  // ディレクトリの位置をもとに戻す
  wrap_chdir("..");
  
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 画像に依存している重みベクトルを作成する関数
// 
int CacheGenerator::make_weight_vector_file() {
  std::vector<std::string> samples;
  DIR *dir_ptr;
  struct stat stat_buf;
  struct dirent *entry;

  // ./$kFeatureVectorDir/ のファイルオープン
  if ((dir_ptr = opendir(kFeatureVectorDir.c_str())) == NULL) {
    return 1;
  }

  // cd
  wrap_chdir(kFeatureVectorDir.c_str());

  // サンプルリストに加える
  while ((entry = readdir(dir_ptr)) != NULL) {
    lstat(entry->d_name, &stat_buf);

    // ファイルがディレクトリの場合は飛ばす
    if (S_ISDIR(stat_buf.st_mode)) continue;

    // オープン(read)先のパスを作成
    std::string source = entry->d_name;

    // 考慮するサンプルに追加
    samples.push_back(source);
  }
  closedir(dir_ptr);

  // cd
  wrap_chdir("..");
  wrap_chdir("..");

  // source のファイルを読み込み、配列化しておく
  // TODO: この処理は関数化したほうがよい
  typedef std::vector<double> Vec;
  std::vector<Vec> feature_vector;
  for (unsigned i = 0; i < samples.size(); ++i) {
    FILE *fp;
    std::string src = kFeatureVectorDir + "/" + samples[i];
    if ((fp = fopen(src.c_str(), "r")) != NULL) {
      double d;
      Vec vec;
      while (fscanf(fp, "%lf", &d) != -1) vec.push_back(d);
      feature_vector.push_back(vec);
      fclose(fp);
    } else {
      printf("ファイルオープン : %s に失敗した.\n", samples[i].c_str());
      return 1;
    }
  }
  
  // ランダム選択用の配列を作成し初期化
  std::vector<int> indice(samples.size(), 0);
  for (int i = 0; i < indice.size(); ++i) indice[i] = i;

  for (unsigned i = 0; i < samples.size(); ++i) {
    // キャッシュ作成先のパスが存在する場合は飛ばす
    std::string destination = kWeightVectorDir + "/" + samples[i] + ".w";
    if (is_exist(destination.c_str())) continue;
    printf("%s\n", destination.c_str());
    
    // ランダムに混ぜる
    std::random_shuffle(indice.begin(), indice.end());
    puts("random shuffle");

    // 先頭から kSize だけをサンプルとして扱う
    const int kSize = 10000;

    // svm の初期設定
    const int K = 1 + (10000 - 1);
    SvmWrapper svm(1, NULL, false);
    svm.problem.l = K;
    svm.problem.y = new double[K];
    svm.problem.x = new svm_node * [K];
    puts("svm init");
    
    // ポジティブ(自分)の設定
    svm.problem.x[0] = new svm_node[kTotalDim + 1];
    for (int j = 0; j < kTotalDim; ++j) {
      svm.problem.x[0][j].index = j + 1;
      svm.problem.x[0][j].value = feature_vector[i][j];
    }
    svm.problem.x[0][kTotalDim].index = -1;
    svm.problem.y[0] = 1;
    puts("positive set");
    
    // ネガティブの設定
    for (int j = 0, idx = 1; idx < K; ++j) {
      if (indice[j] == (int)i) continue;
      svm.problem.x[idx] = new svm_node[kTotalDim + 1];
      for (int k = 0; k < kTotalDim; ++k) {
        svm.problem.x[idx][k].index = k + 1;
        svm.problem.x[idx][k].value = feature_vector[indice[j]][k];
      }
      svm.problem.x[idx][kTotalDim].index = -1;
      svm.problem.y[idx] = -1;
      ++idx;
    }

    puts("negative set");

    // ファイルへ出力
    puts("waiting for SvmWrapper::get_weight_vector()");
    Vec t = svm.get_weight_vector();
    printf("size = %d\n", t.size());
    FILE *file_ptr;
    if ((file_ptr = fopen(destination.c_str(), "w")) != NULL) {
      for (unsigned j = 0; j < kTotalDim; ++j) {
        if (j) fputc(' ', file_ptr);
        fprintf(file_ptr, "%f", t[j]);
      }
      fclose(file_ptr);
    } else {
      printf("ファイル書き込みエラー : %s\n", destination.c_str());
      return 1;
    }

    printf("%s is created successfully.\n", destination.c_str());
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
