#ifndef VISUALIZE_HOG_H
#define VISUALIZE_HOG_H

#include <vector>

#include <opencv2/opencv.hpp>

class VisualizeHog {
  typedef std::vector<double> Vec;
  typedef std::vector<Vec> Mat;
  typedef std::vector<Mat> SMat;
  
  static void calculate_visual_weight(double *feat, std::vector<Mat>& weight);
  static void calculate_visual_weight(const std::vector<double>& feat,
                                      std::vector<Mat>& weight);
 public:
  static cv::Mat generate_mat(double *feat);
  static cv::Mat generate_mat(const std::vector<double>& feat);
};

#endif // VISUALIZE_HOG_H
