#include <string>
#include "driver.h"

int main(int argc, const char *argv[]) {
  auto input = argv[1];

    pache::driver drv(argv[1]);
    drv.parse();
}
