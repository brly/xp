
#include "svm_wrapper.h"

#include <memory>
#include <iostream>

int main(int argc, char **argv) {
  SvmWrapper svm(argc, argv);
  svm.run();
  
  return 0;
}
