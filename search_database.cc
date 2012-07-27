#include "search_database.h"
#include "hog.h"
#include "constant.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>

// static メンバの実体
std::vector<SearchDatabase::ResultPair> SearchDatabase::result;
std::vector<SearchDatabase::Vec> SearchDatabase::database_vector;
std::vector<std::string> SearchDatabase::database_string;

void SearchDatabase::calc_in_database(const std::vector<double>& wq,
                                      const int top_k) {
  // 順位キューを使って上位 top_k 番目までのランキングを求める
  std::priority_queue<SearchDatabase::ResultPair,
                      std::vector<SearchDatabase::ResultPair>,
                      std::greater<SearchDatabase::ResultPair> > pq;

  for (unsigned i = 0; i < database_vector.size(); ++i) {
    double d = Util::get_vector_dot(wq, database_vector[i]);
    if (pq.size() < top_k) {
      pq.push(SearchDatabase::ResultPair(d, database_string[i]));
    } else if (pq.top().first < d) {
      pq.pop();
      pq.push(SearchDatabase::ResultPair(d, database_string[i]));
    }
  }
  
  // 検索結果に追加
  while (!pq.empty()) {
    result.push_back(pq.top());
    pq.pop();
  }
  std::reverse(result.begin(), result.end());
}

void SearchDatabase::search(const std::vector<double>& wq, const int top_k) {
  // 初期化
  result.clear();
  
  // データベース上でそれぞれ類似度の計算
  calc_in_database(wq, top_k);

  // 上位 k 番目までのランキングを出力
  for (int i = 0; i < top_k; ++i) {
    SearchDatabase::ResultPair rp = result.at(i);
    printf("%3d : %30s : %f\n",i , rp.second.c_str(), rp.first);
  }
}

void SearchDatabase::init() {
  // ./$kFeatureVectorDir/ 配下の画像リストを取得
  Util::get_file_list(kFeatureVectorDir, database_string, true);

  // ベクトルデータを保存
  for (unsigned i = 0; i < database_string.size(); ++i) {
    std::vector<double> t;
    Util::read_vector_data(database_string[i], t);
    database_vector.push_back(t);
  }
}
