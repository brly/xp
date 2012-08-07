#include "experiment_manager.h"
#include "constant.h"
#include "random_function.h"
#include "util.h"

#include "base_method.h"
#include "simple_method.h"
#include "linear_combination_method.h"
#include "assembling_method.h"
#include "normal_method.h"

#include <algorithm>
#include <memory>
#include <cassert>
#include <cstdio>

namespace {

const int kMethodN = 5;
const std::string methods[] = {
  "Base", "Simple", "Linear Combination", "Assembling", "Normal"
};

Method* create_method_object(const int type, const std::string& query) {
  switch (type) {
    case 0: return new BaseMethod(query);
    case 1: return new SimpleMethod(query);
    case 2: return new LinearCombinationMethod(query);
    case 3: return new AssemblingMethod(query, 50, 30);
    case 4: return new NormalMethod(query);
  }
  return nullptr;
}

int count_diff(const std::vector<std::string>& lhs,
               const std::vector<std::string>& rhs) {
  if (lhs.size() != rhs.size()) {
    puts("lhs size is diffrent from rhs size.");
    assert(false);
  }
  int ret = 0;
  for (unsigned i = 0; i < lhs.size(); ++i) {
    if (rhs.end() == std::find(rhs.begin(), rhs.end(), lhs[i])) {
      // printf("%s\n", lhs[i].c_str());
      ++ret;
    }
  }
  return ret;
}
 
}; // anonymous namespace

void ExperimentManager::run() {
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
      method.reset();
    }
  }

  // 最後の出力
  for (int i = 0; i < kMethodN; ++i) {
    printf("%s , df {%d}, df2 {%d}, time %f\n",
           methods[i].c_str(),
           diff[i],
           diff2[i],
           avg_time[i]);
  }
}

void ExperimentManager::set_random_query() {
  Util::get_file_list(kImageDir, query, true);
  std::random_shuffle(query.begin(), query.end(), RandomFunction());
  query.erase(query.begin() + kQueryN, query.end());
}
