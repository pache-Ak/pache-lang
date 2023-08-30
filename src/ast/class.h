#ifndef CLASS_H
#define CLASS_H

#include "ast.h"
#include "function.h"
#include "statement.h"
#include <memory>
#include <vector>

namespace pache {
class class_body {
public:
  std::vector<std::unique_ptr<let_stmt>> var_def;
  std::vector<std::unique_ptr<func_ast>> func_def;
  std::vector<std::unique_ptr<class_ast>> inner_class_def;
};
class class_ast : public base_ast {
public:
  explicit class_ast(std::string &&name, class_body &&body);

  // virtual llvm::Value *codegen() override { return nullptr; } // TODO
  std::string const get_name() const { return m_name; }
  // virtual std::string const decorated_name() const override {
  //   return get_name();
  // }

  std::string const m_name;
  class_body m_body;
};

} // namespace pache

#endif
