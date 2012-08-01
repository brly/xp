#include "base_method.h"
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

  for (;;) {
    bool flag = false;

    // 時間計測オブジェクト
    // Timer t("process:");
    clock_t begin = clock();
    
    puts("input method type[0-3a-c,x]");
    
    switch (getchar()) {
      case '0': {
        BaseMethod base_method(kImageDir + "/circle_0.jpg");
        base_method.run();
        break;
      }
      case '1': {
        SimpleMethod simple_method(kImageDir + "/circle_0.jpg");
        simple_method.run();
        break;
      }
      case '2': {
        LinearCombinationMethod linear_combination_method(kImageDir + "/circle_0.jpg");
        linear_combination_method.run();
        break;
      }
      case '3': {
        AssemblingMethod assembling_method(kImageDir + "/circle_0.jpg", 50, 30);
        assembling_method.run();
        break;
      }
      case 'x': {
        BaseMethod base_method(kImageDir + "/caltech101/ant/image_0001.jpg");
        base_method.run();
        break;
      }
      case 'a': {
        SimpleMethod simple_method(kImageDir + "/caltech101/ant/image_0001.jpg");
        simple_method.run();
        break;
      }
      case 'b': {
        LinearCombinationMethod
            linear_combination_method(kImageDir +
                                      "/caltech101/ant/image_0001.jpg");
        linear_combination_method.run();
        break;
      }
      case 'c' : {
        AssemblingMethod
            assembling_method(kImageDir + "/caltech101/ant/image_0001.jpg",
                              50, 30);
        assembling_method.run();        
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
}
