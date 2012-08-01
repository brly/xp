#ifndef SEARCH_DATABASE_H_
#define SEARCH_DATABASE_H_

// "Search Database" という名前のクラスだが、
// 検索の際には、クエリとなる、"クエリの特徴ベクトルの重み" wqが必要となるため
// 名前の割には汎用的ではない事に注意する
// そのため、クラス名の変更が考えられるが、良い名前を思いつかなかった

#include <vector>
#include <string>
#include <utility>

class SearchDatabase {
  typedef std::pair<double, std::string> ResultPair;
  typedef std::vector<double> Vec;

  static std::vector<ResultPair> result;
  static std::vector<Vec> database_vector;
  static std::vector<std::string> database_string;
  
  static void calc_in_database(const std::vector<double>& wq, const int top_k);
  static void show_images(const std::vector<std::string>& files);
  
 public:
  static void search(const std::vector<double>& wq, const int top_k = 20);
  static void init();
};

#endif // SEARCH_DATABASE_H_
