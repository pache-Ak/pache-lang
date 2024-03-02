#include <iostream>
#include <string>
#include "driver.h"
#include "IRbuild/build.h"
#include "llvm/Support/raw_ostream.h"
#include "IRbuild/comp_unit.h"

int main(int argc, const char *argv[]) {
  llvm::InitializeNativeTarget();
  llvm::InitializeNativeTargetAsmPrinter();
  llvm::InitializeNativeTargetAsmParser();
  std::string input;
  pache::InitializeModuleAndManagers();
  if (argc == 2) {
  input = argv[1];

  } 
std::cout<< "begin!\n";
    pache::driver drv(input);
    drv.parse();
    drv.print();
    
    auto ir = build_file(*drv.root_ast);
    pache::TheModule->print(llvm::errs(), nullptr);
}
