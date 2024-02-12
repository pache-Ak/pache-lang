#ifndef SCOPE_H
#define SCOPE_H

#include "ast.h"
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace pache {
  class scope_ast;
  class named_ast  final{
public:
  explicit named_ast(std::unique_ptr<scope_ast> &&scope, std::string_view identifier)
      : m_scope(std::move(scope)), m_iden(identifier) {}

  std::string_view get_name() const;

private:
  std::unique_ptr<scope_ast> m_scope;
  std::string_view m_iden;
};

class scope_ast : public base_ast {
public:
  virtual ~scope_ast() = default;
};

class unqualified_scope_ast final : public scope_ast {
public:
  virtual ~unqualified_scope_ast() = default;
}; 

class qualified_scope_ast : public scope_ast {
public:
  virtual ~qualified_scope_ast() = default ;
private:
};

class relative_scope_ast final : public qualified_scope_ast {
public:
  explicit relative_scope_ast(named_ast &&ast) : m_iden(std::move(ast)){}
explicit relative_scope_ast(std::unique_ptr<scope_ast> &&father, std::string_view name)
  : m_iden(std::move(father), name){}

private:
  named_ast m_iden;
};

class root_scope_ast final : public qualified_scope_ast{
public:
  virtual ~root_scope_ast() = default;
private:
};
}

#endif
