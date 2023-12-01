#include "function_type.h"
#include "IRbuild/type.h"
#include "ast/type.h"
#include "llvm/IR/Type.h"
#include <algorithm>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace pache {
llvm::Type *get_llvm(std::unique_ptr<build_type> const &type) {
  return type->get_llvm_type();
}

void function_type::set_mutable() {
  std::cerr << "can't fix function with mut.";
}
void function_type::set_volatile() {
  std::cerr << "can't fix function with volatile.";
}

std::unique_ptr<build_type>
build_func_type(base_build &father, func_type_ast const &ast) {
  std::vector<std::unique_ptr<build_type>> args_type;
  for (auto &p: ast.get_args_type()) {
    args_type.emplace_back(type_build(father, *p));
  }
  std::vector<llvm::Type *> args_llvm_type;
  std::transform(args_type.begin(), args_type.end(),
                 std::back_inserter(args_llvm_type), get_llvm);
  auto return_type = type_build(father, *ast.get_return_type());

  auto llvm_type = llvm::FunctionType::get(return_type->get_llvm_type(),
                                        args_llvm_type, false);
  return std::make_unique<function_type>(std::move(return_type), 
  std::move(args_type),llvm_type);
}
function_type::function_type(
    std::unique_ptr<build_type> &&return_type,
    std::vector<std::unique_ptr<build_type>> &&args_type,
    llvm::FunctionType *llvm_type)
    : m_return_type(std::move(return_type)), m_args_type(std::move(args_type)),
      m_llvm_type(llvm_type) {}
function_type::function_type(
    std::unique_ptr<build_type> &&return_type,
    std::vector<std::unique_ptr<build_type>> &&args_type)
    : m_return_type(std::move(return_type)), m_args_type(std::move(args_type)) {
  m_llvm_type = llvm::FunctionType::get(
      m_return_type->get_llvm_type(),
      get_llvm_type_arr(m_args_type.begin(), m_args_type.end()), false);
}
} // namespace pache