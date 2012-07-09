//
// Rectangle PGM Image Generator
//

#include <cstdio>
#include <cassert>
#include <cmath>
#include <ctime>
#include <string>
#include <algorithm>
#include <vector>

////////////////////////////////////////////////////////////////////////////////
// pgm画像作成 - 基底クラス
class PgmImageGenerator {
 protected:
  // ファイルハンドル
  FILE *handle_;
  // ファイル名の前につける修飾
  std::string prefix_;
  // 作成する画像数
  int number_;

  // PGMファイルのヘッダを書き込む
  void print_header(int width, int height) {
    fprintf(handle_, "P5\n");
    fprintf(handle_, "# Comments\n");
    fprintf(handle_, "%d %d\n", width, height);
    fprintf(handle_, "255\n");
  }
  
  // アクセサ
  std::string prefix() const { return prefix_; }
  int number() const { return number_; }

  // ファイルへ模様を書き込む
  virtual void print_body(int width, int height, bool swap) = 0;
  
 public:
  
  PgmImageGenerator(const std::string& prefix, const int number)
      : handle_(NULL), prefix_(prefix), number_(number) {}
  
  virtual ~PgmImageGenerator() {}

  // インターフェース関数
  void run();
};

void PgmImageGenerator::run() {
  srand((unsigned)time(NULL));
  
  char format[128];
  int t = 1, compare = 1;
  while (number_ > compare) { ++t; compare *= 10; }
  sprintf(format, "%%s_%%0%dd.pgm", t);

  char file_name[128];
  int width = -1, height = -1;
  for (int i = 0; i < number_; ++i) {
    sprintf(file_name, format, prefix_.c_str(), i);
    handle_ = fopen(file_name, "wb");
    if (handle_ != NULL) {
      width = 100 + rand() % 200, height = 100 + rand() % 200;
      print_header(width, height);
      print_body(width, height, i < number_/2);
      fclose(handle_);
    }
  }
}

///////////////////////////////////////////////////////////////////////////////
// 長方形作成クラス
class RectanglePgmImageGenerator : public PgmImageGenerator {
  void print_body(int width, int height, bool swap);
 public:
  explicit RectanglePgmImageGenerator(const std::string& prefix = "rectangle",
                                      const int number = 500)
      : PgmImageGenerator(prefix, number) {}
  ~RectanglePgmImageGenerator() {}
  
};

void RectanglePgmImageGenerator::print_body(int width, int height, bool swap) {
  int sequence[4];
  // try to make good rectangle
  for (int t = 0; t < 100; ++t) {
    for (int i = 0; i < 4; ++i) sequence[i] = 10 + rand() % (std::min(width, height) - 20);
    std::sort(sequence, sequence + 4);    
    if ((sequence[2] - sequence[0]) * (sequence[3] - sequence[1]) >= 1000)
      break;
  }
  
  int write_color = -1, colors[2] = { 255, 0 };
  if (swap) std::swap(colors[0], colors[1]);
  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      if ((sequence[0] <= i) && (i <= sequence[2]) &&
          (sequence[1] <= j) && (j <= sequence[3])) {
        write_color = colors[0];
      } else {
        write_color = colors[1];
      }
      if (putc(write_color, handle_) == EOF) {
        fprintf(stderr, "putc() return EOF");
        assert(false);
      }
    }
  }
}

class TrianglePgmImageGenerator : public PgmImageGenerator {
  // 3点を求める関数
  void calc_triple_coordinate(std::vector<int>& x, std::vector<int>& y,
                              const int width, const int height);
  void print_body(int width, int height, bool swap);
 public:
  explicit TrianglePgmImageGenerator(const std::string& prefix = "triangle",
                                     const int number = 500)
      : PgmImageGenerator(prefix, number) {}
  ~TrianglePgmImageGenerator() {}
};

void TrianglePgmImageGenerator::calc_triple_coordinate(std::vector<int>& x,
                                                       std::vector<int>& y,
                                                       const int width,
                                                       const int height) {
  // 初期化
  x.resize(3);
  y.resize(3);

  // 求める
  for (int t = 0; t < 100; ++t) {
    // 左下から決定
    x[0] = rand() % (width - 20);
    y[0] = std::min(rand() % (height) + 30, height - 10);
    // 右下
    x[1] = std::min(x[0] + rand() % (width - 20) + 20 , width - 10);
    y[1] = y[0];
    // 上
    x[2] = rand() % (x[1] - x[0]) + x[0];
    y[2] = std::max(y[0] - rand() % (height - 30) - 20, 10);

    int dx = x[1] - x[0], dy = y[2] - y[0];
    if (abs(dx - dy) < 20 && 30 < dx && dy < 30 ) break;
  }
}

void TrianglePgmImageGenerator::print_body(int width, int height, bool swap) {
  // 3点
  std::vector<int> x, y;
  calc_triple_coordinate(x, y, width, height);
  
  // 基準となる面積を計算
  double BaseS = fabs((x[0] - x[2])*(y[1]-y[0]) - (x[0] - x[1])*(y[2] - y[0]) / 2.0);

  int write_color = -1, colors[2] = { 255, 0 };
  if (swap) std::swap(colors[0], colors[1]);
  
  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      // 面積を計算
      double S1 = fabs(((x[0] - j)*(y[1]-y[0]) - (x[0] - x[1])*(i - y[0])) / 2.0);
      double S2 = fabs(((x[0] - x[2])*(i-y[0]) - (x[0] - j)*(y[2] - y[0])) / 2.0);
      double S3 = fabs(((j - x[2])*(y[1]-i) - (j - x[1])*(y[2] - i)) / 2.0);

      // 判定
      if (BaseS + 1e-10 > S1 + S2 + S3) 
        write_color = colors[0];
      else
        write_color = colors[1];

      if (putc(write_color, handle_) == EOF) {
        fprintf(stderr, "putc() return EOF");
        assert(false);
      }
    }
  }
}

class CirclePgmImageGenerator : public PgmImageGenerator {
  void print_body(int width, int height, bool swap);
 public:
  CirclePgmImageGenerator(const std::string& prefix = "circle",
                          const int number = 500)
      : PgmImageGenerator(prefix, number) {}
  ~CirclePgmImageGenerator() {}
};

void CirclePgmImageGenerator::print_body(int width, int height, bool swap) {
  int x, y, r;
  for (int t = 0; t < 100; ++t) {
    x = rand() % (width - 20) + 10;
    y = rand() % (height - 20) + 10;
    r = std::min(rand() % std::min(width, height),
                 std::min( std::min(x - 0, width - x),
                           std::min(y - 0, height - y) ) );
    if (r > 20) break;
  }
  
  int write_color = -1, colors[2] = { 255, 0 };
  if (swap) std::swap(colors[0], colors[1]);

  for (int i = 0; i < height; ++i) {
    for (int j = 0; j < width; ++j) {
      int dx = j - x, dy = i - y;
      write_color = (r*r >= dy*dy + dx*dx) ? colors[0] : colors[1];
      if (putc(write_color, handle_) == EOF) {
        fprintf(stderr, "putc() return EOF");        
        assert(false);
      }
    }
  }
}

int main() {
  // rectangle
  RectanglePgmImageGenerator rpig("rectangle", 5000);
  rpig.run();
  // triangle
  TrianglePgmImageGenerator tpig("triangle", 5000);
  tpig.run();
  // circle
  // CirclePgmImageGenerator cpig("cir", 10);
  // cpig.run();
  return 0;
}
