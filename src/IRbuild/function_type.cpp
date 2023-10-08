#include "function_type.h"
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
template <class Iterator>
function_type::function_type(std::unique_ptr<build_type> &&return_type,
                             Iterator begin, Iterator end)
    : m_return_type(std::move(return_type)), m_args_type(begin, end) {
  std::vector<llvm::Type *> args_llvm_type;
  std::transform(m_args_type.begin(), m_args_type.end(),
                 std::back_inserter(args_llvm_type), get_llvm);
  m_llvm_type = llvm::FunctionType::get(m_return_type->get_llvm_type(),
                                        args_llvm_type, false);
}
void function_type::set_const() {
  std::cerr << "can't fix function with const.";
}
void function_type::set_volatile() {
  std::cerr << "can't fix function with volatile.";
}
} // namespace pache