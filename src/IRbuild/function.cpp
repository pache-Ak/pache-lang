#include "function.h"
#include "build.h"
#include "function_type.h"
#include <algorithm>
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pache {

std::string_view name_mangling(func_ast const &func) {

  std::string decorated_name = get_father_decorated_name(func.get_father());

  // +
  //                              std::to_string(func->get_name().size()) +
  //                              func->get_name();
  // for (auto &type : func->get_args_type()) {
  //   //  decorated_name += type->decorated_name();
  // }
  return func.get_name();
}

std::unique_ptr<build_variable> function_build::find_var(std::string_view name) const {
  if (auto it = std::find_if(var.begin(), var.end(),
    [=](std::pair<std::string_view, std::unique_ptr<build_variable>> const &v){
      return v.first == name;
    }); it != var.end()) {
      return it->second->clone();
    }
}


function_build::function_build(base_build *const father, func_ast const &func)
  : base_build(father), decorated_name(name_mangling(func)), m_type(*father, func.get_type()) {
  llvm::Function *F = llvm::Function::Create(
      m_type.get_llvm_type(), llvm::Function::ExternalLinkage, decorated_name, TheModule.get());

  unsigned Idx = 0;
  for (auto &Arg : F->args()) {
    llvm::AllocaInst *alloca = Builder->CreateAlloca(m_type.get_args_type()[Idx]->get_llvm_type(), nullptr, func.get_args_name()[Idx]);
    Builder->CreateStore(&Arg, alloca);
    var.emplace_back(func.get_args_name()[Idx], std::make_unique<build_local_variable>(m_type.get_args_type()[Idx]->clone(), alloca));
    ++Idx; 
  }

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", F);
  Builder->SetInsertPoint(BB);

  block_scope func_block{father, nullptr};
  std::vector<build_variable> vars;

  for (auto const &args : func.get_args()) {
    auto type = type_build(*father, *args.first.get());

    llvm::AllocaInst *all =
        IR::Builder->CreateAlloca(type->get_llvm_type(), nullptr, args.second);

    func_block.insert(
        std::move(args.second),
        std::make_unique<build_local_variable>(std::move(type), all));
  }

  for (auto &stmt : func.get_block()->get_stmt_list()) {
    statement_build(func_block, *stmt);
  }
  verifyFunction(*F);
  TheFPM->run(*F);
  m_llvm_function = F;

  }

} // namespace pache