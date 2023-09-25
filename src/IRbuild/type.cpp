#include "type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <memory>

namespace pache {
llvm::Type *arr_type::get_llvm_type() const {
  return llvm::ArrayType::get(m_element_type->get_llvm_type(), m_size);
}

void primary_type::set_const() {
  if (is_const()) {
    std::cerr << "redef const.\n";
  } else {
    m_is_const = true;
  }
}

void primary_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}

void pointer_type::set_const() {
  if (is_const()) {
    std::cerr << "redef const.\n";
  } else {
    m_is_const = true;
  }
}

void pointer_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}

void arr_type::set_const() { std::cerr << "con't fix array with const.\n"; }

void arr_type::set_volatile() {
  std::cerr << "con't fix array with volatile.\n";
}

llvm::Type *pointer_type::get_llvm_type() const {
  return llvm::PointerType::get(m_element_type->get_llvm_type(), 0);
}
std::unique_ptr<build_type> type_build(base_build *const father,
                                       type_ast const *const ast) {
  return ast->build(father);
}
std::unique_ptr<reference_type>
build_reference_type(base_build *const father, reference_ast const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  return std::unique_ptr<reference_type>{
      std::make_unique<reference_type>(std::move(element))};
}

std::unique_ptr<build_type> build_const_type(base_build *const father,
                                             const_type const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  element->set_const();
  return element;
}
std::unique_ptr<build_type>
build_volatile_type(base_build *const father, volatile_type const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  element->set_volatile();
  return element;
}
} // namespace pache