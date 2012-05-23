// Hog特徴量を求める
// 現在のところ、外部から利用されるときは、関数に
// 画像のファイル名を渡して、特徴量のベクトルをヒープに確保し、
// そのポインタを返す

#ifndef HOG_H
#define HOG_H

#include <string>
#include <vector>

#include <opencv/cv.h>
#include <opencv/highgui.h>

// HoG特徴量の計算時における各パラメータ

enum {
  //1セル内の横画素数
  kCellX = 5,
  //1セル内の縦画素数
  kCellY = 5,
  //輝度勾配方向の分割数（普通は９）
  kCellBin = 9,
  //1ブロック内の横セル数
  kBlockX = 3,
  //1ブロック内の縦セル数
  kBlockY = 3,
  //リサイズ後の画像の横幅
  kResizeX = 40,
  //リサイズ後の画像の縦幅
  
  kResizeY = 40,
  //セルの数（横）
  kCellWidth = kResizeX / kCellX,
  //セルの数（縦）
  kCellHeight = kResizeY / kCellY,
  //ブロックの数（横）
  kBlockWidth = kCellWidth - kBlockX + 1,
  //ブロックの数（縦）
  kBlockHeight = kCellHeight - kBlockY + 1,
  //１ブロックの特徴量次元
  kBlockDim	= kBlockX * kBlockY * kCellBin,
  //HoG全体の次元
  kTotalDim	= kBlockDim * kBlockWidth * kBlockHeight,
};

class Hog {
  static void get_feature_impl(IplImage *src, double *feat);
 public:
  static double *get_feature(const char *name);
  static double *get_feature(const std::string& name);
};

// HoG特徴量を計算
// @img : グレースケール画像
// @feat : 計算された特徴量が入る

// インターフェース
// @name : 画像ファイル名
// Warning : 返り値のポインタはスマポで受け取るようにしている
// (そもそもスマポを返せば良いのか?)

// void get_hog(const char* name, std::vector<double> feat) {
//   IplImage *img = cvLoadImage(name, 0);
//   double feature_dest[kTotalDim];
//   GetHoG(img, feature_dest);
//   feat.reserve(kTotalDim);
//   // O(N)
//   for (unsigned int i = 0; i < kTotalDim; ++i) feat[i] = feature_dest[i];
//   cvReleaseImage(&img);
// }

#endif // HOG_H
