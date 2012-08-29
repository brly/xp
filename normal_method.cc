
#include "normal_method.h"
#include "hog.h"
#include "search_database.h"
<<<<<<< HEAD
#include "timer.h"
=======
>>>>>>> origin/master

void NormalMethod::run() {
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
  SearchDatabase::search(h.get_data());
}

void NormalMethod::run(std::vector<std::string>& ranking) {
<<<<<<< HEAD
  std::vector<double> data;
  {
    Timer timer("normal");
    Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
    data = h.get_data();
  }
  
  // SearchDatabase::search(data, ranking, 10);
  SearchDatabase::search(data, ranking, 0);
=======
  Hog h(query_.c_str(), kCellX, kBlockX, kResizeX, kResizeY, kOrientation);
  
  SearchDatabase::search(h.get_data(), ranking, 10);
>>>>>>> origin/master
}
