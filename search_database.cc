#include "search_database.h"
#include "hog.h"
#include "constant.h"

#include <iostream>
#include <fstream>
#include <algorithm>

// static メンバの実体
std::vector<SearchDatabase::ResultPair> SearchDatabase::result;

void SearchDatabase::calc_in_database(const char* database_file,
                                      const std::vector<double>& wq) {
  std::ifstream stream(database_file);
  std::string line;
  while (std::getline(stream, line)) {
    Hog hog(line.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    double sum = 0;
    for (int i = 0; i < kTotalDim; ++i) sum += wq[i] * hog[i];
    result.push_back(SearchDatabase::ResultPair(sum, line));
  }
}

void SearchDatabase::search(const std::vector<double>& wq, const int top_k) {
  // 初期化
  result.clear();
  
  // データベース上でそれぞれ類似度の計算
  calc_in_database("POSITIVE_sample", wq);
  calc_in_database("NEGATIVE_sample", wq);

  // 上位 k 番目までのランキングを出力
  std::partial_sort(result.begin(), result.begin() + top_k,
                    result.end(), std::greater<SearchDatabase::ResultPair>());
  for (int i = 0; i < top_k; ++i) {
    SearchDatabase::ResultPair rp = result.at(i);
    printf("%3d : %30s : %f\n",i , rp.second.c_str(), rp.first);
  }
}
