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
<<<<<<< HEAD
    const std::string& query, const int k) {
=======
    const std::string& query) {

>>>>>>> origin/master
  Mat ws;
  std::vector<std::string> vs;
  Util::get_file_list(kWeightVectorDir, vs, true);

  std::random_shuffle(vs.begin(), vs.end(), RandomFunction());
<<<<<<< HEAD
  for (int i = 0; i < k; ++i) {
    Vec t;
=======
  const int k = 5;
  for (int i = 0; i < k; ++i) {
    Vec t;
    // printf("%s\n", vs[i].c_str());
>>>>>>> origin/master
    Util::read_vector_data(vs[i], t);
    ws.push_back(t);
  }
  
  return ws;
}

