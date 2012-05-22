//
// hog.h ユニットテスト
//

#include "hog.h"

#include <vector>

int main() {
  const char name[] = "ant.pgm";
  std::vector<double> feat(100);
  get_hog(name, feat);
  for (unsigned int i = 0; i < kTotalDim; ++i)
    printf("%lf\n", feat[i]);
  return 0;
}
