#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "class.h"
#include "expression.h"
#include "function.h"
#include "statement.h"
#include "type.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace pache {
class class_ast;
class compunit_ast : public base_ast {
public:
  explicit compunit_ast() {}
  void insert_class_def(std::unique_ptr<class_ast> &&p) {
    p->set_father(this->get_father());
    class_def.push_back(std::move(p));
  }

  void insert_func_def(std::unique_ptr<func_ast> &&p) {
    m_func_asts.emplace_back(std::move(p));
  }

  void insert_dec(std::unique_ptr<base_ast> &&p) {
    m_var_table.emplace_back(std::move(p));
  }
  std::vector<std::unique_ptr<func_ast>> const &get_funcs_ast() const {
    return m_func_asts;
  }
  virtual ~compunit_ast() = default;

private:
  std::vector<std::unique_ptr<base_ast>> m_var_table;
  // std::vector<std::pair<std::string, variable_ast *>> dec_name;
  // std::unordered_map<std::string, variable_ast *> m_symbol_table;
  std::vector<std::unique_ptr<class_ast>> class_def;
  // std::unordered_map<std::string, llvm::AllocaInst *> m_named_values;
  std::vector<std::unique_ptr<func_ast>> m_func_asts;
  // std::unordered_multimap<std::string, std::unique_ptr<callable_ast>>
  // m_func_table;
  std::string m_file_name;
  std::vector<std::string> m_lines;
};

} // namespace pache

#endif