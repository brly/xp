
#ifndef NORMAL_METHOD_H_
#define NORMAL_METHOD_H_

#include "method.h"
#include "constant.h"

#include <vector>

class NormalMethod : public Method {
  std::string query_;
 public:
  explicit NormalMethod(const std::string& query)
      : query_(query) {}
  
  void run();
  void run(std::vector<std::string>& ranking);
};

#endif // NORMAL_METHOD_H_
