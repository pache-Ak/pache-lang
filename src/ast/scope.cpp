#include "scope.h"
#include <iostream>
#include <memory>
#include "IRbuild/scope.h"
namespace pache {
unqualified_scope_ast::~unqualified_scope_ast(){}
std::string_view named_ast::get_name() const { return m_iden; }
void unqualified_scope_ast::print() const {
  std::cout << "unqualified_scope_ast\n";
}
void relative_scope_ast::print() const {
  
  std::cout << "relative_scope_ast\n";
  m_iden.print();
}
void root_scope_ast::print() const { std::cout << "root_scope_ast\n"; }

std::unique_ptr<build_scope>
unqualified_scope_ast::build(base_build  &build) const {
  return build_unqualified_scope(build, *this);
}
std::unique_ptr<build_scope>
relative_scope_ast::build(base_build  &build) const {
  return build_relative_scope(build, *this);
}
std::unique_ptr<build_scope>
root_scope_ast::build(base_build  &build) const {
  return build_root_scope(build, *this);
}
void named_ast::print() const {
  m_scope->print();
  std::cout << "m_iden:\t" << m_iden << "\n";
}
} // namespace pache
