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

void class_type::define_body(base_build &build, class_ast const &ast) {
  if (!m_type->isOpaque()) {
    std::cerr << "define class is not opaque!\n"
              << "redefine class:\t" << ast.get_name() << "\n"; 
  }
  
  std::size_t i{0};
  std::vector<llvm::Type *> vars_type;
  for (auto const &let : ast.get_var_def()) {
    auto [it, b] = define_data_members(build, *let, i);
    if (b) {
      vars_type.emplace_back(it->second.get_type().get_llvm_type());
      ++i;
    } else {
      // TODO log error;
      break;
    }
  }

  if (vars_type.empty()) {
    vars_type.emplace_back(pache::Builder->getInt8Ty());
  }
  
  m_type->setBody(vars_type);
}

// return is succseed ture is succseed false is not
std::pair<std::unordered_map<std::string_view, class_type::data_member>::iterator, bool>
class_type::define_data_members(base_build &build, let_stmt const &ast, std::size_t i) {
  auto type = type_build(build, ast.get_var_type());
  if (type == nullptr) {
    std::cerr << "build type error.\n";
    return std::make_pair(m_member_var.end(), false);
  }

  auto pair = m_member_var.try_emplace(ast.get_var_name(), std::move(type), i);

  return pair;
}
} // namespace pache