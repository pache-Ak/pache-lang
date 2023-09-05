#ifndef IR_FUNCTION_H
#define IR_FUNCTION_H

#include "../ast/ast.h"
#include "../ast/function.h"
#include "../ast/type.h"
#include "build.h"
#include "comp_unit.h"
#include <string>
#include <vector>

using namespace std::literals;

namespace pache {
std::string name_mangling(func_ast *func) {
  std::string decorated_name = get_father_decorated_name(func->get_father()) +
                               std::to_string(func->get_name().size()) +
                               func->get_name();
  for (auto &type : func->get_args_type()) {
    //  decorated_name += type->decorated_name();
  }
  return decorated_name;
}
// std::string function_build(base_build *father, func_ast *func) {

//   std::vector<llvm::Type *> args_Type;
//   for (auto &args : func->get_args_type()) {
//     //   args_Type.push_back(args->codegen());
//   }

//   //  llvm::FunctionType *FT = llvm::FunctionType::get(
//   ///     func->get_return_type()->codegen(), args_Type, false);
//   // llvm::Function *F =
//   //     llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
//   //                            name_mangling(func), TheModule.get());

//   unsigned Idx = 0;
//   // for (auto &Arg : F->args()) {
//   //   Arg.setName(func->get_args_name()[Idx]);
//   // }

//   // llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry",
//   F);
//   // Builder->SetInsertPoint(BB);

//   // for (auto &arg : F->args()) {
//   //   m_block.insert(arg->getName(), &arg);
//   // }
//   // m_block.build(func->get_block());
//   // verifyFunction(*TheFunction);
// }

class function_build : public base_build {
public:
  explicit function_build(base_build *father, func_ast *func)
      : base_build(father) {}
  explicit function_build(base_build *father, std::string const &name,
                          std::vector<build_type *> &&args);
  explicit function_build();

  virtual build_variable *const
  find_var(std::string const &name) const override;

  virtual build_type *const find_type(std::string const &name) const override;
  virtual llvm::BasicBlock *get_loop_begin() override { return nullptr; }
  virtual llvm::BasicBlock *get_loop_end() override { return nullptr; }

private:
  std::string decorated_name;
  std::vector<pache::build_type *> args;
};
} // namespace pache

#endif
