#include "simple_method.h"
#include "assembling_method.h"
#include "linear_combination_method.h"

#include <memory>
#include <iostream>

int main(int argc, char **argv) {
  puts("input method type[1-2]");
  switch (getchar()) {
    case '1': {
      SimpleMethod simple_method(argc, argv);
      simple_method.run();      
      break;
    }
    case '2': {
      LinearCombinationMethod linear_combination_method;
      linear_combination_method.run();
      break;
    }
    case 'w': {
      AssemblingMethod assembling_method(50, 30);
      assembling_method.run();
      break;
    }
  }
  return 0;
}
