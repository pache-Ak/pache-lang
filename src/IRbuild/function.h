#ifndef IR_FUNCTION_H
#define IR_FUNCTION_H

#include "../ast/ast.h"
#include "../ast/function.h"
#include "../ast/type.h"
#include "build.h"
#include "function_type.h"
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
std::string name_mangling(std::unique_ptr<func_ast> const &func) {
  // std::string decorated_name = get_father_decorated_name(func->get_father())
  // +
  //                              std::to_string(func->get_name().size()) +
  //                              func->get_name();
  // for (auto &type : func->get_args_type()) {
  //   //  decorated_name += type->decorated_name();
  // }
  return func->get_name();
}

class function_build : public base_build {
public:
  explicit function_build(base_build *father,
                          std::unique_ptr<func_ast> const &func)
      : base_build(father), decorated_name(name_mangling(func)) {

    std::vector<llvm::Type *> args_Type;

    std::vector<std::unique_ptr<build_type>> args_types;

    for (auto &ast : func->get_args_type()) {
      args_types.emplace_back(type_build(father, ast.get()));
    }
    for (auto &type : args_types) {
      args_Type.emplace_back(type->get_llvm_type());
    }

    std::unique_ptr<build_type> ret_type =
        type_build(father, func->get_return_type());

    llvm::FunctionType *FT =
        llvm::FunctionType::get(ret_type->get_llvm_type(), args_Type, false);

    m_type = std::make_unique<function_type>(std::move(ret_type),
                                             std::move(args_types), FT);
    llvm::Function *F = llvm::Function::Create(
        FT, llvm::Function::ExternalLinkage, decorated_name, TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
      Arg.setName(func->get_args_name()[Idx]);
    }

    llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
    Builder->SetInsertPoint(BB);

    block_scope func_block{father, nullptr};
    std::vector<build_variable> vars;

    for (auto &args : func->get_args()) {
      auto type = type_build(father, args.first.get());

      llvm::AllocaInst *all = IR::Builder->CreateAlloca(type->get_llvm_type(),
                                                        nullptr, args.second);

      func_block.insert(
          std::move(args.second),
          std::make_unique<build_local_variable>(std::move(type), all));
    }

    for (auto &stmt : func->get_block()->get_stmt_list()) {
      statement_build(&func_block, stmt.get());
    }
    verifyFunction(*F);
  }
  explicit function_build(base_build *father, std::string const &name,
                          std::unique_ptr<function_type> &&type,
                          llvm::Function *llvm_function)
      : base_build(father), decorated_name(name), m_type(std::move(type)),
        m_llvm_function(llvm_function) {}

  function_build(function_build &&other) = default;

  virtual std::unique_ptr<build_variable> const &
  find_var(std::string const &name) const override;

  virtual build_type *const find_type(std::string const &name) const override;
  virtual ~function_build() = default;
  virtual llvm::Function *get_llvm_function() const { return m_llvm_function; }
  std::unique_ptr<function_type> const &get_function_type() const {
    return m_type;
  }

private:
  std::string decorated_name;
  std::unique_ptr<function_type> m_type;
  llvm::Function *m_llvm_function;
};
} // namespace pache

#endif
