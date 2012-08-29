#ifndef SIMPLE_REPRESENT_VECTOR_FACTORY_H_
#define SIMPLE_REPRESENT_VECTOR_FACTORY_H_

#include "svm_wrapper.h"

#include <vector>
#include <string>

class SimpleRepresentVectorFactory {
  typedef std::vector<double> Vec;
  typedef std::vector<Vec> Mat;
  
 public:
<<<<<<< HEAD
  static Mat create_represent_vector(const std::string& query,
                                     const int k = 5);
=======
  static Mat create_represent_vector(const std::string& query);
>>>>>>> origin/master
};

#endif // SIMPLE_REPRESENT_VECTOR_FACTORY_H_
