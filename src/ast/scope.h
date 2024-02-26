#ifndef AST_SCOPE_H
#define AST_SCOPE_H

#include "IRbuild/build.h"
#include "ast.h"
#include "reference_ptr.h"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pache {
class scope_ast;

class named_ast  final{
public:
  named_ast() = default;
  explicit named_ast(std::unique_ptr<scope_ast> &&scope, std::string && identifier)
      : m_scope(std::move(scope)), m_iden(std::move(identifier)) {}

  std::string_view get_name() const;
  scope_ast const &get_father_scope() const {
    return *m_scope;
  }

private:
  std::unique_ptr<scope_ast> m_scope;
  std::string m_iden;
};

class scope_ast : public base_ast {
public:
  virtual ~scope_ast() = default;
  virtual reference_ptr<base_build >
  build(base_build  &build) const = 0;
};

class unqualified_scope_ast final : public scope_ast {
public:
  virtual ~unqualified_scope_ast();
  virtual reference_ptr<base_build >
  build(base_build  &build) const override;
  virtual void print() const override;
}; 

class qualified_scope_ast : public scope_ast {
public:
  virtual ~qualified_scope_ast() = default ;
private:
};

class relative_scope_ast final : public qualified_scope_ast {
public:
  explicit relative_scope_ast(named_ast &&ast) : m_iden(std::move(ast)){}
explicit relative_scope_ast(std::unique_ptr<scope_ast> &&father, std::string &&name)
  : m_iden(std::move(father), std::move(name)){}

virtual void print() const override;
  virtual ~relative_scope_ast() = default;
  virtual reference_ptr<base_build >
  build(base_build  &build) const override;
  scope_ast const &get_father_scope() const {
    return m_iden.get_father_scope();
  }
  std::string_view get_name() const {
    return m_iden.get_name();
  }
private:
  named_ast m_iden;
};

class root_scope_ast final : public qualified_scope_ast{
public:
  virtual ~root_scope_ast() = default;
  virtual void print() const override;
  virtual reference_ptr<base_build >
  build(base_build  &build) const override;
private:
};
}

#endif
