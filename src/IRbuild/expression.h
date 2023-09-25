#ifndef IR_EXPRESSION_H
#define IR_EXPRESSION_H

#include "../ast/expression.h"
#include "build.h"
#include "function.h"
#include "type.h"
#include <memory>

namespace pache {
template <class Iterator>
std::vector<std::reference_wrapper<std::unique_ptr<function_build> const>>
ADL(std::string const &function_name, Iterator begin, Iterator end);

template <class Iterator>
std::unique_ptr<function_build> const &
function_lookup(std::string const &name, Iterator begin, Iterator end) {
  std::vector<std::reference_wrapper<std::unique_ptr<function_build> const>>
      set = ADL("operator+"s, begin, end);

  ///  TODO

  return nullptr;
}

llvm::Value *build_exp(base_build *father, exp_ast const *const ast);

llvm::Value *build_call_exp(base_build *father,
                            func_call_exp const *const ast) {
  /*  auto val = father->find_var(ast->get_name());
   if (val != nullptr) {
     // val should be a function type variable;

     // then get function
     return;
   }
   std::string name{}; // encoding name
   llvm::Function *CalleeF = TheModule->getFunction(name);
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
   } */
}
} // namespace pache

#endif
