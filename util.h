#ifndef UTIL_H_
#define UTIL_H_

// 頻繁に使用される、汎用的な関数をクラスのスタティック関数として
// 用意しておく

#include <string>
#include <vector>

struct Util {
  // パスを引数として引き取り、destination へ、ファイルのリストを保存して返す
  // ls コマンドに似ている関数
  static void get_file_list(const std::string& dir_path,
                            std::vector<std::string>& destination);

  // コンパイラの警告(-Wunused-result)を避けるための chdir 関数ラッパ
  // クラス外から呼び出さないほうが無難 (戻すのを忘れてしまうため) 
  static void wrap_chdir(const char* path);

  // 数値のみが記述されているデータファイルの読み込みを行う
  // 型は double に限定し、読み込みの終了判定も fscanf() の戻り値が
  // EOF (-1) になった時終了とする
  static void read_vector_data(const std::string& path,
                               std::vector<double>& destination);

  // Linux における、pwd コマンドの実装
  static void pwd();

  // std::vector<double> 同士の内積を返す
  static double get_vector_dot(const std::vector<double>& lhs,
                               const std::vector<double>& rhs);
};

#endif // UTIL_H_

