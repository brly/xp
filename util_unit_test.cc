#include "util.h"

#include <vector>
#include <string>

int main() {
  using namespace std;

  vector<string> vs;
  Util::get_file_list("cache", vs, true);

  for (int i = 0; i < vs.size(); ++i)
    printf("%s\n", vs[i].c_str());
  
  return 0;
}
