#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "class.h"
#include "function.h"
#include "import.h"
#include "statement.h"
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace pache {
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

  void insert_dec(std::unique_ptr<let_stmt> &&p) {
    m_var_table.emplace_back(std::move(p));
  }
  std::vector<std::unique_ptr<func_ast>> const &get_funcs_ast() const {
    return m_func_asts;
  }
  virtual ~compunit_ast() = default;
  std::vector<std::unique_ptr<import_ast>> const &get_packages() const {
    return m_packages;
  }
  std::vector<std::unique_ptr<class_ast>> const &get_class() const {
    return class_def;
  }

  std::vector<std::unique_ptr<let_stmt>> const &get_var() const {
    return m_var_table;
  }

private:
  std::vector<std::unique_ptr<import_ast>> m_packages;
  std::vector<std::unique_ptr<let_stmt>> m_var_table;
  std::vector<std::unique_ptr<class_ast>> class_def;
  std::vector<std::unique_ptr<func_ast>> m_func_asts;
  std::string m_file_name;
  std::vector<std::string> m_lines;
};

} // namespace pache

#endif