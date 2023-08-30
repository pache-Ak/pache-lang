#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "build.h"

namespace pache {
llvm::Value *build_call_exp(base_build *father, func_call_exp *const ast) {
  auto val = father->find_var(ast->get_name());
  if (val != nullptr) {
    // val should be a function type variable;

    // then get function
    return;
  }
  std::string name{}; // encoding name
  Function *CalleeF = TheModule->getFunction(name);
  if (CalleeF != nullptr) {
    std::vector<Value *> ArgsV;
    for (auto exp : ast->get_args()) {
      if (exp->codegen() != nullptr) {
        ArgsV.push_back(exp->codegen());
      } else {
        // log error
      }
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
  }
}
} // namespace pache