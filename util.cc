#include "util.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <cstring>
#include <dirent.h>
#include <sys/stat.h>

namespace {

const int kUserPermission[3] = { S_IRUSR, S_IWUSR, S_IXUSR };
const int kGroupPermission[3] = { S_IRGRP, S_IWGRP, S_IXGRP };
const int kOtherPermission[3] = { S_IROTH, S_IWOTH, S_IXOTH };

};

////////////////////////////////////////////////////////////////////////////////
// 引数の dir_path は、末尾スラッシュを与えないで下さい
// usage : hogehoge/hoge  #=> ok
//       : hogehoge/fuga/ #=> bad         
void Util::get_file_list(const std::string& dir_path,
                         std::vector<std::string>& destination,
                         const bool recursive) {
  // destination をクリア
  // destination.clear();

  DIR *dir_ptr;
  struct stat stat_buf;
  struct dirent *entry;
  std::vector<std::string> dirs;
  
  // dir_path ディレクトリのオープン
  if ((dir_ptr = opendir(dir_path.c_str())) == NULL) {
    printf("ファイルオープン : %s に失敗.\n", dir_path.c_str());
    assert(false);
  }

  // cd (forward)
  // cnt で どのくらいディレクトリを登ったか数えておく
  int cnt = std::count(dir_path.begin(), dir_path.end(), '/') + 1;
  wrap_chdir(dir_path.c_str());

  // ファイルのグラブ
  while ((entry = readdir(dir_ptr)) != nullptr) {
    // とりあえず、ファイルがディレクトリの場合は飛ばす
    // 再帰的にこの関数を呼ぶようにしたら、中にディレクトリがあってもできそうなもんですね
    lstat(entry->d_name, &stat_buf);

    if (S_ISDIR(stat_buf.st_mode)) {
      if (recursive) {
        if ((strcmp(".", entry->d_name) == 0) ||
            (strcmp("..", entry->d_name) == 0)) continue;
        dirs.push_back(dir_path + "/" + std::string(entry->d_name));
      }
      continue;
    }

    // 保存
    destination.push_back(dir_path + "/" + std::string(entry->d_name));
  }
  closedir(dir_ptr);

  // cd (backward)
  for (int i = 0; i < cnt; ++i) wrap_chdir("..");

  // 再帰的に行う
  if (recursive) {
    for (int i = 0; i < dirs.size(); ++i)
      get_file_list(dirs[i], destination, true);
  }
}

void Util::wrap_chdir(const char* path) {
  if (chdir(path) != 0) {
    printf("chdir(%s) に失敗.\n", path);
    assert(false);
  }
}

void Util::read_vector_data(const std::string& path,
                            std::vector<double>& destination) {
  FILE *file_ptr = nullptr;
  if ((file_ptr = fopen(path.c_str(), "r")) == NULL) {
    pwd();
    printf("ファイルオープン : %s に失敗.\n", path.c_str());
    assert(false);
  }

  // destination 初期化
  destination.clear();

  double d;
  while (fscanf(file_ptr, "%lf", &d) != -1) destination.push_back(d);
  fclose(file_ptr);
}

void Util::write_vector_data(const std::string& path,
                             const std::vector<double>& source) {
  FILE *file_ptr = nullptr;
  if ((file_ptr = fopen(path.c_str(), "w")) == NULL) {
    pwd();
    printf("ファイルオープン : %s に失敗.\n", path.c_str());
    assert(false);
  }

  // 書き込み
  for (unsigned i = 0; i < source.size(); ++i) {
    if (i) fputc(' ', file_ptr);
    fprintf(file_ptr, "%f", source[i]);
  }
  
  fclose(file_ptr);
}

void Util::pwd() {
  char buf[1000];
  if (getcwd(buf, sizeof(buf)) == NULL) {
    puts("fail getcwd()");
    return;
  }
  printf("working directry is \"%s\"\n", buf);
}

double Util::get_vector_dot(const std::vector<double>& lhs,
                            const std::vector<double>& rhs) {
  if (lhs.size() != rhs.size()) {
    printf("vector のサイズが異なります. %d - %d.\n", lhs.size(), rhs.size());
    assert(false);
  }

  // 内積を計算
  double ret = 0;
  for (unsigned i = 0; i < lhs.size(); ++i)
    ret += lhs[i] * rhs[i];
  
  return ret;
}

void Util::mkdir_rec(std::string path) {
  std::vector<std::string> vs;
  do {
    path.erase(path.rfind('/'));
    vs.push_back(path);
  } while (path.find('/') != -1);

  std::reverse(vs.begin(), vs.end());
  for (unsigned j = 0; j < vs.size(); ++j) {
    if (is_exist(vs[j].c_str())) continue;
    if (mkdir(vs[j].c_str(), get_permission(755)) != 0) {
      printf("mkdir(%s) に失敗しました.\n",
             vs[j].c_str());
    }
  }
}

bool Util::is_exist(const char* path) {
  // stat 関数用オブジェクト
  struct stat stat_buf;
  return stat(path, &stat_buf) == 0;
}

////////////////////////////////////////////////////////////////////////////////
// 10進数のパーミッションを引数にとり、(例:755, 700 ..)
// 実際のパーミッションに設定されるintの値を返す
int Util::get_permission(int permission) {
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
