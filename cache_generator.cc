
// キャッシュ可能なデータをのキャッシュを作成するプログラム
// 該当するデータ
// - 画像の特徴ベクトル
// - 画像の重みベクトル

#include "hog.h"
#include "constant.h"

#include <iostream>
#include <string>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <cerrno>
#include <sys/stat.h>
#include <dirent.h>

namespace {

const std::string kCacheDir = "cache";
const std::string kFeatureVectorDir = kCacheDir + "/feature_vector";
const std::string kImageDir = "easy_test";

const int kUserPermission[3] = { S_IRUSR, S_IWUSR, S_IXUSR };
const int kGroupPermission[3] = { S_IRGRP, S_IWGRP, S_IXGRP };
const int kOtherPermission[3] = { S_IROTH, S_IWOTH, S_IXOTH };

};

void run();
int directory_check();
int get_permission(int permission);
bool is_exist(const char* path);
int make_feature_vector_file();

void run() {
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

  // 正常に終了
  puts("正常に終了しました.");
}

int directory_check() {
  // ./cache/ が存在するか
  if (!is_exist(kCacheDir.c_str())) {
    int ret = mkdir(kCacheDir.c_str(), get_permission(775));
    // mkdir "cache" が失敗した場合
    if (ret != 0) {
      printf("mkdir %s に失敗.", kCacheDir.c_str());
      return 1;
    }    
  }

  // ./cache/feature_vector/ が存在するか
  if (!is_exist(kFeatureVectorDir.c_str())) {
    int ret = mkdir(kFeatureVectorDir.c_str(), get_permission(775));
    // mkdir "cache/feature_vector" が失敗した場合
    if (ret != 0) {
      printf("mkdir %s に失敗.", kFeatureVectorDir.c_str());
      return 1;
    }    
  }

  // ./$kImageDir/ が存在するか
  if (!is_exist(kImageDir.c_str())) {
    printf("画像ファイルを格納しているディレクトリ : %s が見つかりません.\n",
           kImageDir.c_str());
    return 1;
  }
  
  return 0;
}

int get_permission(int permission) {
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

bool is_exist(const char* file) {
  // stat 関数用オブジェクト
  struct stat stat_buf;
  return stat(file, &stat_buf) == 0;
}

int make_feature_vector_file() {
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
  chdir(kImageDir.c_str());

  // ファイルのグラブ
  while ((entry = readdir(dir_ptr)) != NULL) {
    lstat(entry->d_name, &stat_buf);
    
    // ファイルがディレクトリの場合は飛ばす
    if (S_ISDIR(stat_buf.st_mode)) continue;

    // パスを作成
    std::string file = entry->d_name;

    // パスが指定の拡張子を持たない場合は飛ばす
    if (file.find(".pgm") == -1 && file.find(".jpg")) continue;

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
  
  return 0;
}

int main() {
  run();
  return 0;
}
