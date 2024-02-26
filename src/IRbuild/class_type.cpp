#include "class_type.h"
#include "ast/class.h"
#include "ast/type.h"
#include "build.h"
#include "type.h"
#include "llvm/IR/Type.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <iostream>

namespace pache {
// llvm::type
class_type::class_type(base_build &build, class_ast const &ast) {
  std::size_t i{0};
  std::vector<llvm::Type *> vars_type;
  for (auto const &let : ast.get_var_def()) {
    auto [it, b] = m_member_var.try_emplace(
      let->get_var_name(), type_build(build, let->get_var_type()), ++i
    );

    if (b) {
      vars_type.emplace_back(it->second.get_type().get_llvm_type());
    } else {
      // TODO log error;
      break;
    }
  }

  if (vars_type.empty()) {
    vars_type.emplace_back(pache::Builder->getInt8Ty());
  }
  m_type = llvm::StructType::create(*TheContext, vars_type, ast.get_name());
}

void class_type::set_mutable() {   if (!is_const()) {
    std::cerr << "redef mutable.\n";
  } else {
    m_is_const = false;
  } }
void class_type::set_volatile() {   if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  } }
std::unique_ptr<build_type> class_type::clone() const {
  return std::make_unique<class_type>(*this);
}
llvm::StructType *class_type::get_llvm_type() const {
  return m_type;
}

llvm::Value *class_type::get_member_var(llvm::Value *ptr,
                                         std::string_view name) {
  auto it = m_member_var.find(name);
  if (it != m_member_var.end()) {
    return IR::Builder->CreateGEP(
        get_llvm_type(), ptr,
        {IR::Builder->getInt32(0), IR::Builder->getInt32(it->second.get_num())},
        name);
  } else {
    // TODO log error
    return nullptr;
    //return find_var(name)->get_value();
  }
}
class_type::data_member::data_member(std::unique_ptr<build_type>  &&type,
                                      std::size_t i)
    : m_type(std::move(type)), m_num(i) {}
std::size_t const class_type::data_member::get_num() const { return m_num; }
build_type const &class_type::data_member::get_type() const {
  return*m_type;
}

} // namespace pache