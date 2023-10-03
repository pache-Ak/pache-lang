#include "../build/parser.hpp"
#include "IRbuild/comp_unit.h"
#include "ast/ast.h"
#include "ast/compunit.h"
#include "driver.h"
#include <cassert>
#include <cstdio>
#include <iostream>
#include <memory>
#include <string>
using namespace std;

int main(int argc, const char *argv[]) {
  // 解析命令行参数. 测试脚本/评测平台要求你的编译器能接收如下参数:
  // compiler 模式 输入文件 -o 输出文件

  //  assert(argc == 5);
  // auto mode = argv[1];
  auto input = argv[2];
  // auto output = argv[4];

  // 打开输入文件, 并且指定 lexer 在解析的时候读取这个文件
  // yyin = fopen(input, "r");
  // assert(yyin);
  pache::driver drv;
  drv.scan_begin(fopen(input, "r"));
  // 调用 parser 函数, parser 函数会进一步调用 lexer 解析输入文件的
  unique_ptr<pache::compunit_ast> ast = make_unique<pache::compunit_ast>();

  // pache::parser parse(ast);
  //  auto ret = yyparse(ast);
  //  assert(!ret);
  // parse();
  //  输出解析得到的 AST, 其实就是个字符串
  //  std::cout << ast->dump() << std::endl;
  pache::build_file(ast.get());
  // pache::TheModule->print(llvm::errs(), nullptr);
  // drv.scan_end();
  return 0;
}
