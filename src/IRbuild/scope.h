#ifndef SCOPE_H
#define SCOPE_H

#include "../reference_ptr.h"
#include "build.h"
#include <memory>

namespace pache {
class build_scope {
public:
    build_scope(reference_ptr<base_build> &&p) : m_scope(std::move(p)){}

  virtual ~build_scope() = 0;
  virtual reference_ptr<build_variable>
  find_var(std::string_view name) = 0;
  virtual reference_ptr<build_type const> find_type(std::string_view name) const = 0;
  virtual reference_ptr<base_build> find_scope(std::string_view name)  = 0;

protected:
    reference_ptr<base_build> m_scope;
};

class unqualified_scope : public build_scope {
public:
     unqualified_scope(reference_ptr<base_build> &&p) : build_scope(std::move(p)){}

  virtual ~unqualified_scope() = default;
  virtual reference_ptr<build_variable>
  find_var(std::string_view name) {
    return m_scope->find_var(name);
  }
  virtual reference_ptr<build_type const> find_type(std::string_view name) const {
    return m_scope->find_type(name);
  }
  virtual reference_ptr<base_build> find_scope(std::string_view name) {
    return m_scope->find_scope(name);
  }

};

class qualified_scope : public build_scope {
public:
     qualified_scope(reference_ptr<base_build> &&p) : build_scope(std::move(p)){}

  virtual ~qualified_scope() = default;
  virtual reference_ptr<build_variable>
  find_var(std::string_view name) {
    return m_scope->qualified_var_lookup(name);
  }
  virtual reference_ptr<build_type const> find_type(std::string_view name) const {
    return m_scope->qualified_type_lookup(name);
  }
  virtual reference_ptr<base_build> find_scope(std::string_view name) {
    return m_scope->qualified_scope_lookup(name);
  }

};

class unqualified_scope_ast;
class relative_scope_ast;
class root_scope_ast;

std::unique_ptr<build_scope>
build_unqualified_scope(base_build  &build, unqualified_scope_ast const &ast);
std::unique_ptr<build_scope>
build_relative_scope(base_build  &build, relative_scope_ast const &ast);
std::unique_ptr<build_scope>
build_root_scope(base_build  &build, root_scope_ast const &ast);
}

#endif
