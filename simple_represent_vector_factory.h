#ifndef SIMPLE_REPRESENT_VECTOR_FACTORY_H_
#define SIMPLE_REPRESENT_VECTOR_FACTORY_H_

#include "svm_wrapper.h"

#include <vector>
#include <string>

class SimpleRepresentVectorFactory {
  typedef std::vector<double> Vec;
  typedef std::vector<Vec> Mat;
  
 public:
  static Mat create_represent_vector(const std::string& query);
};

#endif // SIMPLE_REPRESENT_VECTOR_FACTORY_H_
