#include "hog.h"

void Hog::make_histgram() {
  // make histogram_ by cell
  for (unsigned y = 0; y < kImageHeight; ++y) {
    for (unsigned x = 0; x < kImageWidth; ++x) {
      double x_grad, y_grad;
      // x
      if (x == 0)
        x_grad = source_.at<uchar>(y, 1) - source_.at<uchar>(y, 0);
      else if (x == kImageWidth - 1)
        x_grad = source_.at<uchar>(y, kImageWidth - 1)
                 - source_.at<uchar>(y, kImageWidth - 2);
      else
        x_grad = source_.at<uchar>(y, x + 1) - source_.at<uchar>(y - 1, x);
      // y
      if (y == 0)
        y_grad = source_.at<uchar>(1, x) - source_.at<uchar>(0, x);
      else if (y == kImageHeight - 1)
        y_grad = source_.at<uchar>(kImageHeight - 1, x)
                 - source_.at<uchar>(kImageHeight - 2, x);
      else
        y_grad = source_.at<uchar>(y + 1, x) - source_.at<uchar>(y - 1, x);
      // params
      double magnitude = std::sqrt(x_grad * x_grad + y_grad * y_grad);
      double gradient = std::atan2(y_grad, x_grad) * 180 / CV_PI;
      double test = gradient;
      if (gradient < 0.0) gradient += 360.0;
      if (180.0 < gradient) gradient -= 180.0;
      gradient = gradient / kAngle;
      const int idx =
          (y / kCellSize) * (kImageWidth / kCellSize) * kOrientation +
          (x / kCellSize) * kOrientation + (static_cast<int>(gradient - 1e-10));
      // store
      if ((idx >= kHistgram) || (static_cast<int>(gradient-1e-10) == 9)) {
        printf("err : %d >= %d\n", idx, kHistgram);
        printf("gradient %f %f\n", test,  gradient);
        assert(false);
      }      
      histogram_[idx] = magnitude;
    }
  }
}

void Hog::make_feature() {
  int fcount = 0;
  // block loop
  for (unsigned y = 0; y < kImageHeight / kCellSize - kBlockSize + 1; ++y) {
    for (unsigned x = 0; x < kImageWidth / kCellSize - kBlockSize + 1; ++x) {
      double sum_magnitude = 0.0;
      // cell loop
      for (unsigned j = 0; j < kBlockSize; ++j) {
        for (unsigned i = 0; i < kBlockSize; ++i) {
          for (unsigned k = 0; k < kOrientation; ++k) {
            const int idx = (y + j) * (kImageWidth / kCellSize) * kOrientation +
                            (x + i) * kOrientation + k;
            if (idx >= kHistgram) {
              printf("err : %d\n", idx);
              assert(idx < kHistgram);
            }
            double tmp = histogram_[idx];
            sum_magnitude += tmp * tmp;
          }
        }
      }
      sum_magnitude =  1.0 / std::sqrt(sum_magnitude + 1.0);
      // cell loop
      for (unsigned j = 0; j < kBlockSize; ++j) {
        for (unsigned i = 0; i < kBlockSize; ++i) {
          for (unsigned k = 0; k < kOrientation; ++k) {
            const int idx = (y + j) * (kImageWidth / kCellSize) * kOrientation +
                            (x + i) * kOrientation +
                            k;
            if (idx >= kHistgram) {
              printf("err : %d\n", idx);
              assert(idx < kHistgram);
            }
            destination_[fcount] = histogram_[idx] * sum_magnitude;
            ++fcount;
          }
        }
      }
    }
  }
}
