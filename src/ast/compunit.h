#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "class.h"
#include "function.h"
#include "import.h"
#include "statement.h"
#include <memory>
#include <string>
#include <vector>
#include <filesystem>

namespace pache {
class compunit_ast final : public base_ast {
public:
  explicit compunit_ast() {}
  void insert_class_def(std::unique_ptr<class_ast> &&p) {
    class_def.emplace_back(std::move(p));
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
  std::vector<std::unique_ptr<import_ast>> const &get_packages() const {
    return m_packages;
  }
  std::vector<std::unique_ptr<class_ast>> const &get_class() const {
    return class_def;
  }

  std::vector<std::unique_ptr<let_stmt>> const &get_var() const {
    return m_var_table;
  }

  std::vector<std::filesystem::path> const &get_include_dir() const {
    return m_include_dir;
  }

  virtual void print() const override;

private:
  std::vector<std::unique_ptr<import_ast>> m_packages;
  std::vector<std::unique_ptr<let_stmt>> m_var_table;
  std::vector<std::unique_ptr<class_ast>> class_def;
  std::vector<std::unique_ptr<func_ast>> m_func_asts;
  std::string m_file_name;
  std::vector<std::string> m_lines;

  static thread_local std::vector<std::filesystem::path> m_include_dir;
};

} // namespace pache

#endif