#include "simple_method.h"
#include "assembling_method.h"

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
      AssemblingMethod assembling_method(argc, argv);
      assembling_method.run();      
      break;
    }
  }
  return 0;
}
