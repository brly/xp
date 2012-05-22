#include <iostream>
#include <fstream>
#include <cstdio>

const std::string positive_file("POSITIVE");
const std::string negative_file("NEGATIVE");
void run() {
  std::string line;

  puts("positive files list.");
  std::ifstream positive_str(positive_file.c_str());
  while (std::getline(positive_str, line))
    printf("%s\n", line.c_str());

  puts("negative files list.");
  std::ifstream negative_str(negative_file.c_str());
  while (std::getline(negative_str, line))
    printf("%s\n", line.c_str());
}

int main() {
  run();
  return 0;
}
