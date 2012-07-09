
#ifndef HOG_H
#define HOG_H

#include <opencv2/opencv.hpp>

#include <iostream>
#include <fstream>
#include <vector>
#include <cstdio>
#include <cmath>

class Hog {
 public:
  // constant
  const unsigned kCellSize;
  const unsigned kBlockSize;
  const unsigned kImageWidth;
  const unsigned kImageHeight;
  const unsigned kOrientation;
  const unsigned kTotalDimension;
  const double kAngle;
  const unsigned kHistgram;
 private:
  // reference of destination of feature
  std::vector<double> destination_;
  // image matrix
  cv::Mat source_;
  // feature of histogram_
  std::vector<double> histogram_;

  void make_histgram();

  void make_feature();

 public:
  // constructor & process
  explicit Hog(const char* name,
               unsigned cell_size, unsigned block_size, unsigned image_width,
               unsigned image_height, unsigned orientation)
      : kCellSize(cell_size),
        kBlockSize(block_size),
        kImageWidth(image_width),
        kImageHeight(image_height),
        kOrientation(orientation),
        kTotalDimension((kImageWidth / kCellSize - kBlockSize + 1) *
                        (kImageHeight / kCellSize - kBlockSize + 1) *
                        (kBlockSize * kBlockSize * kOrientation)),
        kAngle(180.0 / (double)kOrientation),
        kHistgram((kImageWidth / kCellSize) * (kImageHeight / kCellSize)
                  * kOrientation),
        destination_(kTotalDimension),
        source_(cv::imread(name, 0)),        
        histogram_(kHistgram, 0) {
    // resize image
    cv::resize(source_, source_, cv::Size(kImageWidth, kImageHeight));
    make_histgram();
    make_feature();
  }

  double operator[](unsigned idx) const {
    return destination_.at(idx);
  }
};

#endif // HOG_H
