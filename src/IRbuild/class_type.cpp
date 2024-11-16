#include "class_type.h"
#include "IRbuild/variable.h"
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
std::unique_ptr<build_type> class_type::clone() const {
  return std::make_unique<class_type>(*this);
}
llvm::StructType *class_type::get_llvm_type() const {
  return m_type;
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
std::unique_ptr<build_variable>
class_type::get_member_var(llvm::Value *obj,
                                       std::string_view name) const {
  if (obj->getType() != m_type) {
    std::cerr << "object's type is not same as this class type.\n";
  }

  if (auto it = m_member_var.find(name); it != m_member_var.end()) {

    llvm::Value *offset =
        Builder->CreateStructGEP(obj->getType(), obj, it->second.get_num());
    llvm::Value *var =
        Builder->CreateLoad(it->second.get_type().get_llvm_type(), offset);
    return std::make_unique<build_prvalue_variable>(it->second.get_type().clone(),
                                                  var);
  } else {
    return nullptr;
  }
}
} // namespace pache