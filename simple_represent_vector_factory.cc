#include "simple_represent_vector_factory.h"
#include "hog.h"
#include "constant.h"
#include "random_function.h"
#include "util.h"

#include <iostream>
#include <fstream>
#include <algorithm>
#include <memory>
#include <cassert>

SimpleRepresentVectorFactory::Mat
SimpleRepresentVectorFactory::create_represent_vector(
    const std::string& query, const int k) {
  Mat ws;
  std::vector<std::string> vs;
  Util::get_file_list(kWeightVectorDir, vs, true);

  RandomFunction r;
  std::random_shuffle(vs.begin(), vs.end(), r);
  for (int i = 0; i < k; ++i) {
    Vec t;
    Util::read_vector_data(vs[i], t);
    ws.push_back(t);
  }
  
  return ws;
}

