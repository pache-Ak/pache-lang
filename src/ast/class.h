#ifndef CLASS_H
#define CLASS_H

#include "ast.h"
#include "function.h"
#include "statement.h"
#include <memory>
#include <string>
#include <string_view>
#include <vector>

namespace pache {

class class_ast final : public base_ast {
public:
  class class_body {
  public:
    std::vector<std::unique_ptr<let_stmt>> var_def;
    std::vector<std::unique_ptr<func_ast>> func_def;
    std::vector<std::unique_ptr<class_ast>> inner_class_def;
  };
  explicit class_ast(std::string && name, class_body &&body);

  std::string_view  get_name() const { return m_name; }

  std::vector<std::unique_ptr<let_stmt>> const &get_var_def() const {
    return m_body.var_def;
  }
  std::vector<std::unique_ptr<class_ast>> const &get_inner_class_def() const {
    return m_body.inner_class_def;
  }
  std::vector<std::unique_ptr<func_ast>> const &get_func_def() const {
    return m_body.func_def;
  }
  virtual void print() const override;

private:
  std::string m_name;
  class_body m_body;
};

} // namespace pache

#endif
