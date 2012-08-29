#include "experiment_manager.h"
#include "constant.h"
#include "random_function.h"
#include "util.h"

#include "base_method.h"
#include "simple_method.h"
#include "linear_combination_method.h"
#include "assembling_method.h"
#include "normal_method.h"
<<<<<<< HEAD
#include "c_method.h"
=======
>>>>>>> origin/master

#include <algorithm>
#include <memory>
#include <cassert>
#include <cstdio>

<<<<<<< HEAD
#define ENABLE_ANOTHER

namespace {

// another version
#ifdef ENABLE_ANOTHER

const int kMethodN = 1 + 3 + 3 + 6 + 6 + 1;

const std::string methods[] = {
  "Base",
  "Simple 100", "Simple 200", "Simple 500",
  "Linear 100", "Linear 200", "Linear 500",
  "Assembling 100 50", "Assembling 100 25", "Assembling 200 50", "Assembling 25",
  "Assembling 500 50", "Assembling 500 25",
  "CMethod 100 50", "CMethod 100 0",
  "CMethod 200 50", "CMethod 200 0",
  "CMethod 500 50", "CMethod 500 0",
  "Normal"
=======
namespace {

const int kMethodN = 5;
const std::string methods[] = {
  "Base", "Simple", "Linear Combination", "Assembling", "Normal"
>>>>>>> origin/master
};

Method* create_method_object(const int type, const std::string& query) {
  switch (type) {
    case 0: return new BaseMethod(query);
<<<<<<< HEAD
      
    case 1: return new SimpleMethod(query, 100);
    case 2: return new SimpleMethod(query, 200);
    case 3: return new SimpleMethod(query, 500);
      
    case 4: return new LinearCombinationMethod(query, 100);
    case 5: return new LinearCombinationMethod(query, 200);
    case 6: return new LinearCombinationMethod(query, 500);

    case 7: return new AssemblingMethod(query, 100, 50);
    case 8: return new AssemblingMethod(query, 100, 25);
    case 9: return new AssemblingMethod(query, 200, 50);
    case 10: return new AssemblingMethod(query, 200, 25);
    case 11: return new AssemblingMethod(query, 500, 50);
    case 12: return new AssemblingMethod(query, 500, 25);

    case 13: return new CMethod(query, 100, 50);
    case 14: return new CMethod(query, 100, 0);
    case 15: return new CMethod(query, 200, 50);
    case 16: return new CMethod(query, 200, 0);
    case 17: return new CMethod(query, 500, 50);
    case 18: return new CMethod(query, 500, 0);

    case 19: return new NormalMethod(query);
  }
  return nullptr;
}

#else

// normal version
const int kMethodN = 17;
const std::string methods[] = {
  "Base", "Simple", "Linear Combination 100",
  "Linear Combination 200", 
  "Assembling 50 45", "Assembling 50 30", "Assembling 50 15",
  "Assembling 100 45", "Assembling 100 30",  "Assembling 100 15",
  "Assembling 200 45", "Assembling 200 30", "Assembling 200 15",
  "Assembling 500 45", "Assembling 500 30", "Assembling 500 15",
  "Normal"
};
Method* create_method_object(const int type, const std::string& query) {
  switch (type) {
    case 0: return new BaseMethod(query);
    case 1: return new SimpleMethod(query);
    case 2: return new LinearCombinationMethod(query, 100);
    case 3: return new LinearCombinationMethod(query, 200);
    case 4: return new AssemblingMethod(query, 50, 45);
    case 5: return new AssemblingMethod(query, 50, 30);
    case 6: return new AssemblingMethod(query, 50, 15);
    case 7: return new AssemblingMethod(query, 100, 45);
    case 8: return new AssemblingMethod(query, 100, 30);
    case 9: return new AssemblingMethod(query, 100, 15);
    case 10: return new AssemblingMethod(query, 200, 45);
    case 11: return new AssemblingMethod(query, 200, 30);
    case 12: return new AssemblingMethod(query, 200, 15);
    case 13: return new AssemblingMethod(query, 500, 45);
    case 14: return new AssemblingMethod(query, 500, 30);
    case 15: return new AssemblingMethod(query, 500, 15);
    case 16: return new NormalMethod(query);
=======
    case 1: return new SimpleMethod(query);
    case 2: return new LinearCombinationMethod(query);
    case 3: return new AssemblingMethod(query, 50, 30);
    case 4: return new NormalMethod(query);
>>>>>>> origin/master
  }
  return nullptr;
}

<<<<<<< HEAD
#endif // ENABLE_ANOTHER

=======
>>>>>>> origin/master
int count_diff(const std::vector<std::string>& lhs,
               const std::vector<std::string>& rhs) {
  if (lhs.size() != rhs.size()) {
    puts("lhs size is diffrent from rhs size.");
    assert(false);
  }
  int ret = 0;
  for (unsigned i = 0; i < lhs.size(); ++i) {
<<<<<<< HEAD
    // rhs に含まれない要素がある場合は ret を 1 増加する
    if (rhs.end() == std::find(rhs.begin(), rhs.end(), lhs[i]))
      ++ret;
  }
  return ret;
}

////////////////////////////////////////////////////////////////////////////////
// lhs .. 元のランキング
// rhs .. 別の方法による、ランキング
int count_gap(const std::vector<std::string>& lhs,
              const std::vector<std::string>& rhs) {
  int ret = 0;
  for (unsigned i = 0; i < lhs.size(); ++i) {
    std::vector<std::string>::const_iterator it =
        std::find(rhs.begin(), rhs.end(), lhs[i]);
    // 上記の std::find では、要素は必ず rhs の中に存在するので、
    // it の値は rhs.end() を指し示す事はない仮定に基づいている
    ret += abs(i - distance(rhs.begin(), it));
=======
    if (rhs.end() == std::find(rhs.begin(), rhs.end(), lhs[i])) {
      // printf("%s\n", lhs[i].c_str());
      ++ret;
    }
>>>>>>> origin/master
  }
  return ret;
}
 
}; // anonymous namespace

void ExperimentManager::run() {
<<<<<<< HEAD
  // コードリーディングのための typedef
  typedef std::vector<std::string> VectorString;
  
  // 集合の差を保持する配列
  std::vector<int> diff(kMethodN, 0);
  std::vector<int> diff2(kMethodN, 0);

  // ランキングのずれを保持する配列
  std::vector<int> gap(kMethodN, 0);
  std::vector<int> gap2(kMethodN, 0);
  
  for (int i = 0; i < kQueryN; ++i) {
    VectorString base_ranking;
    printf("query -> \"%s\"\n", query[i].c_str());
    for (int j = 0; j < kMethodN; ++j) {
      // clock_t start = clock();
      std::shared_ptr<Method> method(::create_method_object(j, query[i]));
      VectorString ranking;
      if (j == 0) {
        // 比較対象の元となるランキングを取得
        // このランキングは上位10位までとなる
        method->run(base_ranking);
      } else {
        // ランキングを取得
        method->run(ranking);
        // 集合の差を数える
        diff[j] += count_diff(base_ranking,
                              VectorString(ranking.begin(),
                                           ranking.begin() + 10));
        diff2[j] +=
            count_diff(VectorString(base_ranking.begin(),
                                    base_ranking.begin() + 5),
                       VectorString(ranking.begin(),
                                    ranking.begin() + 5));
        // ランキングのずれを数える
        gap[j] += count_gap(base_ranking, ranking);
        gap2[j] +=
            count_gap(VectorString(base_ranking.begin(),
                                   base_ranking.begin() +  5),
                      ranking);
      }
      // 本当は必要ないけど、明示的にメモリ開放文を書く
=======
  std::vector<double> avg_time(kMethodN, 0);
  std::vector<int> diff(kMethodN, 0);
  std::vector<int> diff2(kMethodN, 0);
  for (int i = 0; i < kQueryN; ++i) {
    printf("query %s\n", query[i].c_str());
    std::vector<std::string> base_ranking;
    for (int j = 0; j < kMethodN - 1; ++j) {
      printf("method >> %s : ", methods[j].c_str());
      clock_t start = clock();
      std::shared_ptr<Method> method(::create_method_object(j, query[i]));
      std::vector<std::string> ranking;
      if (j == 0) {
        method->run(base_ranking);
      } else {
        method->run(ranking);
        int ret = count_diff(base_ranking, ranking);
        printf("%d ", ret);
        diff[j] += ret;
        printf("%d \n", ret);
        ret = count_diff(std::vector<std::string>(
                base_ranking.begin(), base_ranking.begin() + 5),
                       std::vector<std::string>(
                           ranking.begin(), ranking.begin() + 5));
        diff2[j] += ret;
      }
      avg_time[j] += (clock() - start) / CLOCKS_PER_SEC;
>>>>>>> origin/master
      method.reset();
    }
  }

  // 最後の出力
  for (int i = 0; i < kMethodN; ++i) {
<<<<<<< HEAD
    printf("\"%s\" %d %d, %d %d\n",
           methods[i].c_str(),
           diff[i], diff2[i],
           gap[i], gap2[i]);
=======
    printf("%s , df {%d}, df2 {%d}, time %f\n",
           methods[i].c_str(),
           diff[i],
           diff2[i],
           avg_time[i]);
>>>>>>> origin/master
  }
}

void ExperimentManager::set_random_query() {
  Util::get_file_list(kImageDir, query, true);
  std::random_shuffle(query.begin(), query.end(), RandomFunction());
  query.erase(query.begin() + kQueryN, query.end());
}
