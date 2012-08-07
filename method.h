#ifndef METHOD_H_
#define METHOD_H_

#include <string>
#include <vector>

class Method {
 public:
  virtual void run() = 0;
  virtual void run(std::vector<std::string>& t) = 0;
};

#endif // METHOD_H_
