#include "hog.h"

void Hog::get_feature_impl(IplImage* src, double* feat) {
  //はじめに、画像サイズを変換（kCellX/Yの倍数とする）
  IplImage* img = cvCreateImage(cvSize(kResizeX,kResizeY), 8, 1);
  cvResize(src, img);

  //画像サイズ
  const int width = kResizeX;
  const int height = kResizeY;
	
  //各セルの輝度勾配ヒストグラム
  double hist[kCellWidth][kCellHeight][kCellBin];
  memset(hist, 0, kCellWidth*kCellHeight*kCellBin*sizeof(double));

  //各ピクセルにおける輝度勾配強度mと勾配方向degを算出し、ヒストグラムへ
  //※端っこのピクセルでは、計算しない
  for(int y=0; y<height; y++){
    for(int x=0; x<width; x++){
      if(x==0 || y==0 || x==width-1 || y==height-1){
        continue;
      }
      double dx = img->imageData[y*img->widthStep+(x+1)]
                  - img->imageData[y*img->widthStep+(x-1)];
      double dy = img->imageData[(y+1)*img->widthStep+x]
                  - img->imageData[(y-1)*img->widthStep+x];
      double m = sqrt(dx*dx+dy*dy);
      
      //0.0～360.0の範囲になるよう変換
      double deg = (atan2(dy, dx)+CV_PI) * 180.0 / CV_PI;
      int bin = kCellBin * deg/360.0;
      if(bin < 0) bin=0;
      if(bin >= kCellBin) bin = kCellBin-1;
      hist[(int)(x/kCellX)][(int)(y/kCellY)][bin] += m;
    }
  }

  //ブロックごとで正規化
  for(int y=0; y<kBlockHeight; y++){
    for(int x=0; x<kBlockWidth; x++){
			
      //このブロックの特徴ベクトル（次元kBlockDim=kBlockX*kBlockY*kCellBin）
      double vec[kBlockDim];
      memset(vec, 0, kBlockDim*sizeof(double));
      for(int j=0; j<kBlockY; j++){
        for(int i=0; i<kBlockX; i++){
          for(int d=0; d<kCellBin; d++){
            int index = j*(kBlockX*kCellBin) + i*kCellBin + d; 
            vec[index] = hist[x+i][y+j][d];
          }
        }
      }

      //ノルムを計算し、正規化
      double norm = 0.0;
      for(int i=0; i<kBlockDim; i++){
        norm += vec[i]*vec[i];
      }
      for(int i=0; i<kBlockDim; i++){
        vec[i] /= sqrt(norm + 1.0);
      }

      //featに代入
      for(int i=0; i<kBlockDim; i++){
        int index = y*kBlockWidth*kBlockDim + x*kBlockDim + i;
        feat[index] = vec[i];
      }
    }
  }
  cvReleaseImage(&img);
  return;
}

double *Hog::get_feature(const char* name) {
  IplImage *img = cvLoadImage(name, 0);
  double *feature_dest = new double[kTotalDim];
  get_feature_impl(img, feature_dest);
  cvReleaseImage(&img);
  return feature_dest;
}

double *Hog::get_feature(const std::string& name) {
  return get_feature(name.c_str());
}
