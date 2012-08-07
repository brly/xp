#ifndef EXPERIMENT_MANAGER_H_
#define EXPERIMENT_MANAGER_H_

#include <string>
#include <vector>

class ExperimentManager {
  const int kQueryN;
  std::vector<std::string> query;
  
  void set_random_query();  
 public:
  explicit ExperimentManager(const int N) : kQueryN(N), query(0) {
    // クエリをN個セットする
    set_random_query();
  }
  void run();
};

#endif // EXPERIMENT_MANAGER_H_
