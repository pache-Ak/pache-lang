#include "function_type.h"
#include "IRbuild/type.h"
#include "ast/type.h"
#include "type.h"
#include "llvm/IR/Type.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <iterator>
#include <memory>
#include <vector>

namespace pache {
llvm::Type *get_llvm(std::unique_ptr<build_type> const &type) {
  return type->get_llvm_type();
}


std::vector<std::unique_ptr<build_type>> args_type_build(base_build &build, std::vector<std::unique_ptr<type_ast>> const &ast) {
  std::vector<std::unique_ptr<build_type>> args_type;
  //using namespace std::placeholders;
  //std::transform(ast.begin(), ast.end(), std::back_inserter(args_type), std::bind(type_build, std::ref(build), _1));
  for (auto const & type : ast) {
    args_type.push_back(type_build(build, *type));
  }
  return args_type;
}

std::vector<llvm::Type *> llvm_type_TR(std::vector<std::unique_ptr<build_type>> const &types) {
  std::vector<llvm::Type *> tr_types;
  if (std::any_of(types.begin(), types.end(), 
  [](std::unique_ptr<build_type> const &type){return type != nullptr;})) {

  //std::transform(types.begin(), types.end(), std::back_inserter(tr_types), get_llvm);
  for (auto const &type : types) {
    tr_types.emplace_back(type->get_llvm_type());
  }
  }
  return tr_types;
}

function_type::function_type(base_build &build, func_type_ast const &ast)
  : m_return_type(type_build(build, ast.get_return_type())),
  m_args_type(args_type_build(build, ast.get_args_type())),
  m_llvm_type([&]()->llvm::FunctionType*{
    if (m_return_type == nullptr || std::any_of(m_args_type.begin(), m_args_type.end(), [](auto const &type){
      return type == nullptr;
    })) {
      return nullptr;
    } else {
   return llvm::FunctionType::get(m_return_type->get_llvm_type(), llvm_type_TR(m_args_type), false); 
    
    }
  }()){}

void function_type::set_mutable() {
  std::cerr << "can't fix function with mut.";
}
void function_type::set_volatile() {
  std::cerr << "can't fix function with volatile.";
}

std::unique_ptr<function_type>
build_func_type(base_build &father, func_type_ast const &ast) {
  std::vector<std::unique_ptr<build_type>> args_type;
  for (auto &p: ast.get_args_type()) {
    args_type.emplace_back(type_build(father, *p));
  }
  std::vector<llvm::Type *> args_llvm_type;
  std::transform(args_type.begin(), args_type.end(),
                 std::back_inserter(args_llvm_type), get_llvm);
  auto return_type = type_build(father, ast.get_return_type());

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
std::unique_ptr<build_type> function_type::clone() const {
  std::vector<std::unique_ptr<build_type>> args;
  for (auto const &arg : m_args_type) {

  args.emplace_back(arg->clone());
  }
  return std::make_unique<function_type>(m_return_type->clone(), std::move(args));
}
} // namespace pache