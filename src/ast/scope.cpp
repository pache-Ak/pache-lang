#include "scope.h"
#include <iostream>
#include "IRbuild/scope.h"
namespace pache {
unqualified_scope_ast::~unqualified_scope_ast(){}
std::string_view named_ast::get_name() const { return m_iden; }
void unqualified_scope_ast::print() const {
  std::cout << "unqualified_scope_ast\n";
}
void relative_scope_ast::print() const {
  std::cout << "relative_scope_ast\n"
            << "m_iden:\t" << m_iden.get_name() << "\n";
}
void root_scope_ast::print() const { std::cout << "root_scope_ast\n"; }

reference_ptr<base_build >
unqualified_scope_ast::build(base_build  &build) const {
  return build_unqualified_scope(build, *this);
}
reference_ptr<base_build >
relative_scope_ast::build(base_build  &build) const {
  return build_relative_scope(build, *this);
}
reference_ptr<base_build >
root_scope_ast::build(base_build  &build) const {
  return build_root_scope(build, *this);
}
} // namespace pache
