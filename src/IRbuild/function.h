#ifndef IR_FUNCTION_H
#define IR_FUNCTION_H

#include "../ast/ast.h"
#include "../ast/function.h"
#include "../ast/type.h"
#include "build.h"
#include "comp_unit.h"
#include "statement.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

using namespace std::literals;

namespace pache {
std::string name_mangling(func_ast const *const func) {
  // std::string decorated_name = get_father_decorated_name(func->get_father())
  // +
  //                              std::to_string(func->get_name().size()) +
  //                              func->get_name();
  // for (auto &type : func->get_args_type()) {
  //   //  decorated_name += type->decorated_name();
  // }
  return func->get_name();
}
std::string function_build(base_build *father, func_ast const *const func) {

  std::vector<llvm::Type *> args_Type;
  {
    std::vector<std::unique_ptr<build_type>> args_types;
    for (auto &ast : func->get_args_type()) {
      args_types.emplace_back(type_build(father, ast.get()));
    }
    for (auto &type : args_types) {
      args_Type.emplace_back(type->get_llvm_type());
    }
  }

  llvm::FunctionType *FT = llvm::FunctionType::get(
      type_build(father, func->get_return_type())->get_llvm_type(), args_Type,
      false);
  llvm::Function *F =
      llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                             name_mangling(func), TheModule.get());

  unsigned Idx = 0;
  for (auto &Arg : F->args()) {
    Arg.setName(func->get_args_name()[Idx]);
  }

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  Builder->SetInsertPoint(BB);

  block_scope func_block{father};
  std::vector<build_variable> vars;

  for (auto &args : func->get_args()) {
    auto type = type_build(father, args.first.get());

    llvm::AllocaInst *all =
        IR::Builder->CreateAlloca(type->get_llvm_type(), nullptr, args.second);

    func_block.insert(args.second, build_local_variable{std::move(type), all});
  }

  for (auto &stmt : func->get_block()->get_stmt_list()) {
    statement_build(&func_block, stmt.get());
  }
  verifyFunction(*F);
}

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
