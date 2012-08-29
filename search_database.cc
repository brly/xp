#include "search_database.h"
#include "hog.h"
#include "constant.h"
#include "util.h"

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <algorithm>
#include <queue>
#include <cstdio>

namespace {

const int extN = 2;
std::string ext[] = {
  ".pgm", ".jpg"
};

};

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
    if (pq.size() < static_cast<unsigned>(top_k)) {
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

void SearchDatabase::show_images(const std::vector<std::string>& files) {
  // 画像ウィンドウ作成
  for (unsigned i = 0; i < files.size(); ++i) {
    cv::Mat m = cv::imread(files[i]);
    cv::namedWindow("test", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
    cv::imshow("test", m);
    cv::waitKey(0);
  }
  // キーウェイト

}

void SearchDatabase::search(const std::vector<double>& wq, const int top_k) {
  // 初期化
  result.clear();
  
  // データベース上でそれぞれ類似度の計算
  calc_in_database(wq, top_k);

  // 上位 k 番目までのランキングを出力
  std::vector<std::string> views;
  for (int i = 0; i < top_k; ++i) {
    SearchDatabase::ResultPair rp = result.at(i);

    // TODO
    views.push_back(rp.second);
    // パス変換
    for (int j = 0; j < 2; ++j) 
      views.back() = views.back().substr(views.back().find('/') + 1);
    views.back() = kImageDir + "/" + views.back();
    // 拡張子を推測
    for (int j = 0; j < extN; ++j) {
      std::string s = views.back() + ext[j];
      if (Util::is_exist(s.c_str())) views.back() = s;
    }

    // printf("%3d : %s : %f\n",i , views.back().c_str(), rp.first);
    printf("%s\n", views.back().c_str());
  }
  // for (unsigned i = 0; i < views.size(); ++i) printf("%s\n", views[i].c_str());
<<<<<<< HEAD
=======

  // 画像を表示
  // これはのちのち改良後につくるとする
  // 今は eog コマンドで代替的に見せる
  // show_images(views);
>>>>>>> origin/master
}

void SearchDatabase::search(const std::vector<double>& wq,
                            std::vector<std::string>& dest, const int top_k) {
  result.clear();
<<<<<<< HEAD
  if (top_k == 0) {
    calc_in_database(wq, database_vector.size());
    for (int i = 0; i < database_vector.size(); ++i) {
      SearchDatabase::ResultPair rp = result.at(i);
      // 拡張子変換とかはあとで
      dest.push_back(rp.second);
    }
  } else {
    calc_in_database(wq, top_k);
    for (int i = 0; i < top_k; ++i) {
    SearchDatabase::ResultPair rp = result.at(i);
    // 拡張子変換とかはあとで
    dest.push_back(rp.second);
    }
=======
  calc_in_database(wq, top_k);
  for (int i = 0; i < top_k; ++i) {
    SearchDatabase::ResultPair rp = result.at(i);
    // 拡張子変換とかはあとで
    dest.push_back(rp.second);
>>>>>>> origin/master
  }
}

void SearchDatabase::init() {
  // 初期化中であることを通知
<<<<<<< HEAD
  // printf("Database is being initialized now.\n");
  
  // ./$kFeatureVectorDir/ 配下の画像リストを取得
  // Util::get_file_list(kFeatureVectorDir + "/caltech101/ant", database_string, true);
  Util::get_file_list(kFeatureVectorDir, database_string, true);
=======
  printf("Database is being initialized now.\n");
  
  // ./$kFeatureVectorDir/ 配下の画像リストを取得
  Util::get_file_list(kFeatureVectorDir + "/caltech101/ant", database_string, true);
>>>>>>> origin/master

  // ベクトルデータを保存
  for (unsigned i = 0; i < database_string.size(); ++i) {
    std::vector<double> t;
    Util::read_vector_data(database_string[i], t);
    database_vector.push_back(t);
  }

  // 初期化処理が完了したことを通知
<<<<<<< HEAD
  // printf("Database was constructed completely.\n");
=======
  printf("Database was constructed completely.\n");
>>>>>>> origin/master
}
