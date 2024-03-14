#include "function.h"
#include "IRbuild/type.h"
#include "ast/function.h"
#include "build.h"
#include "function_type.h"
#include "variable.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <sstream>
#include <optional>

namespace pache {
std::string name_mangling(base_build const &scope, std::string_view name, function_type const &type) {
  using namespace std::literals::string_literals;

  std::string s;
  std::ostringstream oss;
  oss << std::hex << name.size();
  s += scope.decorated_name();
  s += "_F"s;
  s += oss.str();
  s += name;

  for (auto const &t : type.get_args_type()) {
    s += t->decorated_name();
  }

  return s;
}


std::unique_ptr<build_variable> function_build::clone() const {
  auto str = decorated_name;
  return std::make_unique<function_build>(get_type().clone(),
                                              std::move(str), m_llvm_function);
}


std::optional<pache::function_build> forward_statement(base_build &build, func_ast const &ast) {
    auto type = build_func_type(build, ast.get_type());
    if (type == nullptr) {
        return std::nullopt;
    }

    auto name = name_mangling(build, ast.get_name(), *type);

    llvm::Function *F = llvm::Function::Create(
        type->get_llvm_type(), llvm::Function::ExternalLinkage, name, TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
        Arg.setName(ast.get_args_name()[Idx]);
        ++Idx; 
    }

    return std::make_optional<function_build>(std::move(type), std::move(name), F);
}

void function_build::define(base_build *const father, func_ast const &ast) {
  
  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "entry", m_llvm_function);
  Builder->SetInsertPoint(BB);

  block_scope func_block{father};

  auto it_arg = m_llvm_function->args().begin();
  auto it_type = get_type().get_args_type().begin();
  while (it_arg != m_llvm_function->args().end()) {
    llvm::IRBuilder<> TmpB(&m_llvm_function->getEntryBlock(),
                 m_llvm_function->getEntryBlock().begin());
    llvm::AllocaInst *Alloca = TmpB.CreateAlloca((*it_type)->get_llvm_type(), nullptr,
                           it_arg->getName());

    Builder->CreateStore(it_arg, Alloca);
    
    func_block.insert(it_arg->getName(), std::make_unique<build_local_variable>((*it_type)->clone(), Alloca));
    ++it_arg;
    ++it_type;
  }

  for (auto &stmt : ast.get_block()) {
    statement_build(func_block, *stmt);
  }
  verifyFunction(*m_llvm_function, &llvm::errs());
  //TheFPM->run(*m_llvm_function, *TheFAM);
}
} // namespace pache