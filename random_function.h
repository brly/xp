#ifndef RANDOM_FUNCTION_H_
#define RANDOM_FUNCTION_H_

// std::random_shuffle へ渡す関数オブジェクト用クラス

#include <ctime>
#include <cstdlib>

class RandomFunction {
public:
  RandomFunction() { srand(static_cast<unsigned>(time(NULL))); }
  unsigned operator()(unsigned max) {
    const double temp = static_cast<double>(rand()/static_cast<double>(RAND_MAX));
    return static_cast<unsigned>(temp*max);
  }
};

#endif // RANDOM_FUNCTION_H_
