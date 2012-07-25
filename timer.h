#ifndef TIMER_H_
#define TIMER_H_

// 時間計測クラス
// 基本的にスタックオブジェクトとして利用し、関数の計算時間を
// 計測する場合などに利用する.
// 計測した時間はデストラクタが呼ばれる際に標準出力へ表示する.

class Timer {
  std::string process_name_;
  clock_t begin_;
 public:
  explicit Timer(const char* process_nane) : process_name_(process_nane),
                                             begin_(clock()) {}
  ~Timer() {
    printf("%s : %f sec\n",
           process_name_.c_str(),
           static_cast<double>(clock() - begin_) / CLOCKS_PER_SEC);
  }
};

#endif // 
