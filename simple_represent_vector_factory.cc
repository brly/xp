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
    const std::string& query) {

  Mat ws;
  std::vector<std::string> vs;
  Util::get_file_list(kWeightVectorDir, vs, true);

  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());
  const int k = 5;
  for (int i = 0; i < k; ++i) {
    Vec t;
    // printf("%s\n", vs[i].c_str());
    Util::read_vector_data(vs[i], t);
    ws.push_back(t);
  }
  
  return ws;
}

