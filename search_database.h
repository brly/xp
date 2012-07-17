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

  static std::vector<ResultPair> result;
  
  static void calc_in_database(const char* database_file, const std::vector<double>& wq);
 public:
  static void search(const std::vector<double>& wq, const int top_k);
};

#endif // SEARCH_DATABASE_H_
