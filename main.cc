#include "simple_method.h"
#include "assembling_method.h"
#include "linear_combination_method.h"
#include "timer.h"
#include "search_database.h"

#include <memory>
#include <iostream>

int main(int argc, char **argv) {
  // データベースを初期化
  SearchDatabase::init();

  while (1) {
    bool flag = false;

    // 時間計測オブジェクト
    Timer t("process:");
    puts("input method type[1-3]");
    
    switch (getchar()) {
      case '1': {
        SimpleMethod simple_method(argc, argv);
        simple_method.run();      
        break;
      }
      case '2': {
        LinearCombinationMethod linear_combination_method;
        linear_combination_method.run();
        break;
      }
      case '3': {
        AssemblingMethod assembling_method(50, 30);
        assembling_method.run();
        break;
      }
      case 'q': {
        flag = true;
        break;
      }
    }

    // 終了フラグ
    if (flag) break;
  }

  return 0;
}
