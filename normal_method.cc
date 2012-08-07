
#include "normal_method.h"
#include "hog.h"
#include "search_database.h"

void NormalMethod::run() {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
  SearchDatabase::search(h.get_data());
}

void NormalMethod::run(std::vector<std::string>& ranking) {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
  SearchDatabase::search(h.get_data(), ranking, 10);
}
