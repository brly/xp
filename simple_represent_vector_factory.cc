#include "simple_represent_vector_factory.h"
#include "hog.h"
#include "constant.h"
#include "random_function.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <cassert>

namespace {

static void erase_above_directry(std::string& path) {
  unsigned t = path.find('/');
  if (t == -1) return;
  path.erase(0, t+1);
}

};

void SimpleRepresentVectorFactory::select_represent(
    std::vector<std::string>& vs, const std::string& query) {
  // Hard Coding
  std::ifstream db_stream("NEGATIVE_sample");
  std::string line;
  while (std::getline(db_stream, line))
    vs.push_back(line);
  vs.erase(std::remove(vs.begin(), vs.end(), query), vs.end());
  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());

  // k = 5 に仮定 ( vs.size() >> 5 )
  const int k = 5;
  vs.erase(vs.begin() + k, vs.end());
  for (int i = 0; i < k; ++i) printf("%s\n", vs[i].c_str());
}

SimpleRepresentVectorFactory::Mat
SimpleRepresentVectorFactory::create_represent_vector(
    const std::string& query) {
  // 代表にする画像を決定
  std::vector<std::string> represents;
  select_represent(represents, kImageDir + "/" + query);

  // 代表の重みベクトルを求める
  Mat ws;
  for (unsigned i = 0; i < represents.size(); ++i) {
    // 重みベクトルのキャッシュ格納先パスを作成
    std::string path = represents[i];
    path.erase(path.size() - 4);
    erase_above_directry(path);
    path = kWeightVectorDir + "/" + path;
    printf("%s\n", path.c_str());

    // 重みベクトル格納ファイルをオープン
    FILE *file_ptr = nullptr;
    if ((file_ptr = fopen(path.c_str(), "r")) != nullptr) {
      Vec v;
      double d;
      // ファイルからデータ読み込み
      while (fscanf(file_ptr, "%lf", &d) != -1) v.push_back(d);
      // 返り値を求める
      ws.push_back(v);
      fclose(file_ptr);
    } else {
      printf("ファイルオープン : %s に失敗.\n",
             path.c_str());
      assert(false);
    }
  }

  return ws;
}

