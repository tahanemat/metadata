#include <iostream>

#include "directory.h"

int main(int argc, char *argv[]) {

  std::filesystem::path dir{argc > 1 ? argv[1] : "."};
  if (!std::filesystem::exists(dir)) {
    std::cout << "Path " << dir << " does not exist.\n";
    return 1;
  }

  std::filesystem::directory_entry entry{dir};

  print_directory(entry);
  display_all_files_in_entry(entry);

  return 0;
}
