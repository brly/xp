#include "visualize_hog.h"
#include "constant.h"

void VisualizeHog::calculate_visual_weight(double *feat, std::vector<Mat>& weight) {
  for (unsigned y = 0; y < kBlockHeight; ++y) {
    for (unsigned x = 0; x < kBlockWidth; ++x) {
      const unsigned base_idx
          = y * kBlockWidth * kBlockDim + x * kBlockDim;
      for (unsigned j = 0; j < kBlockY; ++j) {
        for (unsigned i = 0; i < kBlockX; ++i) {
          for (unsigned k = 0; k < kCellBin; ++k) {
            const unsigned idx
                = base_idx + j * kBlockX * kCellBin + i * kCellBin + k;
            weight[y + j][x + i][k] += feat[idx];
          }
        }
      }
    }
  }
}

void VisualizeHog::calculate_visual_weight(const std::vector<double>& feat,
                                           std::vector<Mat>& weight) {
  for (unsigned y = 0; y < kBlockHeight; ++y) {
    for (unsigned x = 0; x < kBlockWidth; ++x) {
      const unsigned base_idx
          = y * kBlockWidth * kBlockDim + x * kBlockDim;
      for (unsigned j = 0; j < kBlockY; ++j) {
        for (unsigned i = 0; i < kBlockX; ++i) {
          for (unsigned k = 0; k < kCellBin; ++k) {
            const unsigned idx
                = base_idx + j * kBlockX * kCellBin + i * kCellBin + k;
            weight[y + j][x + i][k] += feat[idx];
          }
        }
      }
    }
  }
}

cv::Mat VisualizeHog::generate_mat(double *feat) {
  assert(false);
  // calculate weight
  std::vector<Mat> weight(8, Mat(8, Vec(9, 0)));
  calculate_visual_weight(feat, weight);

  // make glyph
  cv::Mat base(20, 20, CV_8UC1, cv::Scalar(0, 0, 0));
  for (int i = 0; i < 20; ++i)
    base.at<uchar>(i, 9) = base.at<uchar>(i, 10) = 1;

  // output destination
  cv::Mat od(20 * kCellWidth,
             20 * kCellHeight,
             CV_32FC1,
             cv::Scalar(0, 0, 0));

  // visualize
  for (unsigned y = 0; y < 8; ++y) {
    for (unsigned x = 0; x < 8; ++x) {
      for (int k = 0; k < 9; ++k) {
        float angle = -20.0 * k;
        cv::Point2f center(base.rows * 0.5, base.cols * 0.5);
        const cv::Mat affine_matrix =
            cv::getRotationMatrix2D(center, angle, 1.0f);
        cv::Mat dst;
        cv::warpAffine(base, dst, affine_matrix, base.size());
        // draw
        for (unsigned i = 0; i < 20; ++i) {
          for (unsigned j = 0; j < 20; ++j) {
            od.at<float>(y * 20 + i, x * 20 + j)
                += dst.at<uchar>(i,j) * weight[y][x][k];
          }
        }
      }
    }
  }
  
  return od;
}

cv::Mat VisualizeHog::generate_mat(const std::vector<double>& feat) {
  SMat visual_weight(kBlockHeight, Mat(kBlockWidth, Vec(kOrientation)));

  // calc weight
  for (unsigned y = 0; y < kBlockHeight; ++y) 
    for (unsigned x = 0; x < kBlockWidth; ++x) 
      for (unsigned j = 0; j < kBlockY; ++j) 
        for (unsigned i = 0; i < kBlockX; ++i) 
          for (unsigned k = 0; k < kOrientation; ++k) 
            visual_weight[y][x][k]
                += feat[ y * kBlockWidth * kBlockDim +
                         x * kBlockDim + 
                         j * kBlockX * kOrientation +
                         i * kOrientation +
                         k ];

  // Output destination
  cv::Mat od = cv::Mat::zeros(kGlyphSize * kBlockHeight,
                              kGlyphSize * kBlockWidth,
                              CV_32FC1);

  // Make base-glyph
  cv::Mat base = cv::Mat::zeros(kGlyphSize,
                                kGlyphSize,
                                CV_32FC1);
  
  for (int i = 0; i < kGlyphSize; ++i)
    base.at<float>(i, 9) = base.at<float>(i, 10) = 1;

  for (unsigned y = 0; y < kBlockHeight; ++y) {
    for (unsigned x = 0; x < kBlockWidth; ++x) {
      for (int k = 0; k < kOrientation; ++k) {
        float angle = -20.0 * k;
        cv::Point2f center(base.rows * 0.5, base.cols * 0.5);
        const cv::Mat affine_matrix =
            cv::getRotationMatrix2D(center, angle, 1.0f);
        cv::Mat dst;
        cv::warpAffine(base, dst, affine_matrix, base.size());
        // draw
        for (unsigned i = 0; i < kGlyphSize; ++i) {
          for (unsigned j = 0; j < kGlyphSize; ++j) {
            od.at<float>(y * kGlyphSize + i, x * kGlyphSize + j)
                += dst.at<float>(i,j) * visual_weight[y][x][k];
          }
        }
      }
      // cv::namedWindow("test", CV_WINDOW_AUTOSIZE | CV_WINDOW_FREERATIO);
      // cv::imshow("test", od);
      // cv::waitKey(0);
    }
  }
  
  return od;
}
