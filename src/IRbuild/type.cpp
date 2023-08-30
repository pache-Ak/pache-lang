#include "type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"

llvm::Type *pache::arr_type::codegen() {
  return llvm::ArrayType::get(m_element_type->codegen(), m_size);
}

llvm::Type *pache::pointer_type::codegen() {
  return llvm::PointerType::get(m_element_type->codegen(), 0);
}

void pache::primary_type::set_const() {
  if (is_const) {
    std::cerr << "redef const.\n";
  } else {
    is_const = true;
  }
}

void pache::primary_type::set_volatile() {
  if (is_volatile) {
    std::cerr << "redef volatile.\n";
  } else {
    is_volatile = true;
  }
}

void pache::arr_type::set_const() {
  std::cerr << "con't fix array with const.\n";
}

void pache::arr_type::set_volatile() {
  std::cerr << "con't fix array with volatile.\n";
}

void pache::pointer_type::set_const() {
  if (is_const) {
    std::cerr << "redef const.\n";
  } else {
    is_const = true;
  }
}

void pache::pointer_type::set_volatile() {
  if (is_volatile) {
    std::cerr << "redef volatile.\n";
  } else {
    is_volatile = true;
  }
}