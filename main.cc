
#include "hog.h"
#include "svm_wrapper.h"

#include <memory>
#include <iostream>

int main(int argc, char **argv) {
  // 試験的にHoG
  // std::shared_ptr<double> feat(Hog::get_feature("couple.jpg"));
  
  init_svm();
  get_model();
  dispose_svm();
  return 0;
}
