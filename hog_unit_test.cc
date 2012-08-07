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

#include <opencv2/gpu/gpu.hpp>
#include <opencv2/gpu/gpumat.hpp>

int main() {
  Hog h("easy_test/circle_0.jpg", kCellX, kBlockX, kResizeX, kResizeY, kOrientation);

  std::cout << h.get_data().size() << std::endl;
  
  return 0;
}

