#include "util.h"

#include <algorithm>
#include <cassert>
#include <cstdio>
#include <dirent.h>
#include <sys/stat.h>

////////////////////////////////////////////////////////////////////////////////
// 引数の dir_path は、末尾スラッシュを与えないで下さい
// usage : hogehoge/hoge  #=> ok
//       : hogehoge/fuga/ #=> bad         
void Util::get_file_list(const std::string& dir_path,
                         std::vector<std::string>& destination) {
  // destination をクリア
  destination.clear();

  DIR *dir_ptr;
  struct stat stat_buf;
  struct dirent *entry;
  
  // dir_path ディレクトリのオープン
  if ((dir_ptr = opendir(dir_path.c_str())) == NULL) {
    printf("ファイルオープン : %s に失敗.\n", dir_path.c_str());
    assert(false);
  }

  // cd (forward)
  wrap_chdir(dir_path.c_str());

  // ファイルのグラブ
  while ((entry = readdir(dir_ptr)) != nullptr) {
    // とりあえず、ファイルがディレクトリの場合は飛ばす
    // 再帰的にこの関数を呼ぶようにしたら、中にディレクトリがあってもできそうなもんですね
    lstat(entry->d_name, &stat_buf);
    if (S_ISDIR(stat_buf.st_mode)) continue;

    // 保存
    destination.push_back(dir_path + "/" + std::string(entry->d_name));
  }

  // cd (backward)
  int cnt = std::count(dir_path.begin(), dir_path.end(), '/') + 1;
  for (int i = 0; i < cnt; ++i) wrap_chdir("..");
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
