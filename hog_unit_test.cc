//
// hog.h ユニットテスト
//

#include "hog.h"
#include "constant.h"

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <fstream>


int main() {

  using namespace std;

  ifstream ifs("NEGATIVE_database");
  string line;
  while (ifs >> line) {
    Hog h(line.c_str(), 5, 3, 40, 40, 9);
    // for (unsigned i = 0; i < kTotalDim; ++i)
    //   if (h[i])
    //     printf("%f\n", h[i]);
    printf("%s\n", line.c_str());
  }

  return 0;
}

