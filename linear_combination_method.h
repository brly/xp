#ifndef LINEAR_COMBINATION_METHOD_H_
#define LINEAR_COMBINATION_METHOD_H_

#include "method.h"
#include "constant.h"
#include "simple_represent_vector_factory.h"

#include <vector>
#include <string>
#include <utility>

class LinearCombinationMethod : public Method {
  std::vector<double> wq;
  
  void init_weight_vector();

  void scanning_database_by_wq();
  
 public:
  explicit LinearCombinationMethod() : wq(kTotalDim, 0) {
    init_weight_vector();
  }
  
  void run();
};

#endif // LINEAR_COMBINATION_METHOD_H_
