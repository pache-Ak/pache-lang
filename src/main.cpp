#include <iostream>
#include <string>
#include "driver.h"
#include "IRbuild/build.h"

int main(int argc, const char *argv[]) {
  std::string input;
  pache::InitializeModule();
  if (argc == 2) {
  input = argv[1];

  } 
std::cout<< "begin!\n";
    pache::driver drv(input);
    drv.parse();
    drv.print();
}
