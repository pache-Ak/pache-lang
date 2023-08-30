#ifndef CLASS_H
#define CLASS_H

#include "../ast/function.h"
#include "../ast/statement.h"

#include "build.h"

#include <memory>
#include <memory_resource>

namespace pache {
class class_build : public base_build {
public:
  explicit class_build(
      base_build *father,
      /* std::string const &name,  */ llvm::StructType *llvm_type)
      : base_build(father), /* m_name(name),  */ m_type(llvm_type) {}

  void set_body(std::vector<std::unique_ptr<let_stmt>> &v_var) {
    std::vector<llvm::Type *> vars_type;
    for (auto &stmt_ptr : v_var) {
      // vars_type.push_back(stmt_ptr->get_var_type()->codegen());
    }
    m_type->setBody(vars_type);
    for (auto &stmt_ptr : v_var) {
      unsigned int i = 0;
      auto [it, b] = m_member_var.try_emplace(stmt_ptr->get_var_name(), i);
      if (!b) {
        std::cerr << "file name : line : error : redefinition of "
                  << stmt_ptr->get_var_name() << "\n";
      }
      i += 1;
    }
  }
  llvm::StructType *get_IRtype() const { return m_type; }
  void create_member_functions(std::vector<std::unique_ptr<func_ast>> &v_func);
  virtual llvm::Value *find_var(std::string const &name) const override {
    if (m_father != nullptr) {
      return m_father->find_var(name);
    } else {
      return nullptr;
    }
  }

private:
  llvm::Value *get_member_var(llvm::Value *ptr, std::string const &name) const {
    auto it = m_member_var.find(name);
    if (it != m_member_var.end()) {
      return IR::Builder->CreateGEP(
          m_type, ptr,
          {IR::Builder->getInt32(0), IR::Builder->getInt32(m_member_var[name])},
          name);
    } else {
      return find_var(name);
    }
  }
  std::pmr::unordered_map<std::string, unsigned int> m_member_var;
  // std::string const m_name; // used by log error
  llvm::StructType *m_type;
  std::pmr::unordered_map<std::string, std::unique_ptr<class_build>>
      builded_classes;
};
} // namespace pache

#endif
