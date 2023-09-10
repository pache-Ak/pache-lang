#ifndef IR_CLASS_H
#define IR_CLASS_H

#include "../ast/function.h"
#include "../ast/statement.h"

#include "build.h"
#include <memory>
#include <memory_resource>
#include <string>
#include <unordered_map>

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
      auto p = m_member_var.emplace(stmt_ptr->get_var_name(), i);
      if (!(p.second)) {
        std::cerr << "file name : line : error : redefinition of "
                  << stmt_ptr->get_var_name() << "\n";
      }
      i += 1;
    }
  }
  llvm::StructType *get_IRtype() const { return m_type; }
  void create_member_functions(std::vector<std::unique_ptr<func_ast>> &v_func);
  virtual build_variable *const
  find_var(std::string const &name) const override;

private:
  llvm::Value *get_member_var(llvm::Value *ptr, std::string const &name);

  std::unordered_map<std::string, unsigned int> m_member_var;
  // std::string const m_name; // used by log error
  llvm::StructType *m_type;
  std::unordered_map<std::string, std::unique_ptr<class_build>> builded_classes;
};
} // namespace pache

#endif
