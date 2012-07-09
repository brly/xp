#include "simple_method.h"

#include <memory>
#include <iostream>

int main(int argc, char **argv) {
  SimpleMethod simple_method(argc, argv);
  simple_method.run();
  
  // SvmWrapper svm(argc, argv);
  // svm.run();
  
  return 0;
}
