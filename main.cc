#include "base_method.h"
#include "simple_method.h"
#include "assembling_method.h"
#include "linear_combination_method.h"
#include "normal_method.h"
#include "c_method.h"
#include "timer.h"
#include "search_database.h"
#include "experiment_manager.h"

#include <memory>
#include <iostream>

#define ENABLE_EXPERIMENT_MANAGER

namespace {

std::string methods[] = {
  "Base", "Simple", "Linear combination", "Assembling", "Normal dot"
};
const int methodsN = 5;

std::string querys[] = {
  kImageDir + "/circle_0.jpg",
  kImageDir + "/caltech101/ant/image_0001.jpg"
};
const int querysN = 2;

void set_query(std::string& s) {
  for (int i = 0; i < querysN; ++i)
    printf("%d : %s\n", i, querys[i].c_str());

  int input;
  do {
    fscanf(stdin, " %d", &input);
    if (input < 0 || 1 < input) {
      printf("input is %d!\n", input);
      continue;
    }
    break;
  } while (1);
  s = querys[input];
}

}; // anonymous namespace

int main(int argc, char **argv) {
  // データベースを初期化
  SearchDatabase::init();

#ifdef ENABLE_EXPERIMENT_MANAGER  
  
  ExperimentManager exp(1);
  exp.run();  
  return 0;

#else

  for (;;) {
    // 終了フラグ変数
    bool flag = false;

    // 時間計測
    // もともとは Timer クラスのオブジェクト使ってたけど
    // バグ出てたので、そのうち書きなおす
    // static メソッドにすればいいのかもしれない
    clock_t begin = clock();

    std::string query;
    puts("set query");
    set_query(query);
    
    puts("input method type [0-4]");
    for (int i = 0; i < methodsN; ++i)
      printf("%d : %s\n", i, methods[i].c_str());

    char in;
    if (scanf(" %c", &in) == -1) assert(false);
    
    switch (in) {
      case '0': {
        BaseMethod base_method(query);
        base_method.run();
        break;
      }
      case '1': {
        SimpleMethod simple_method(query);
        simple_method.run();
        break;
      }
      case '2': {
        LinearCombinationMethod linear_combination_method(query);
        linear_combination_method.run();
        break;
      }
      case '3': {
        AssemblingMethod assembling_method(query, 20, 50);
        assembling_method.run();
        break;
      }
      case '4': {
        NormalMethod normal_method(query);
        normal_method.run();
        break;
      }
      case '5': {
        CMethod c_method(query, 100, 50);
        c_method.run();
        break;
      }
      case 'q': {
        flag = true;
        break;
      }
    }
    
    // time
    printf("process : %f\n",
           static_cast<double>(clock() - begin) / CLOCKS_PER_SEC);

    // 終了フラグ
    if (flag) break;
  }
  return 0;
#endif // ENABLE_EXPERIMENT_MANAGER
}
