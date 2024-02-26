#include "scope.h"

namespace pache {
reference_ptr<base_build >
build_unqualified_scope(base_build  &build, unqualified_scope_ast const &ast) {
    return &build;
}
reference_ptr<base_build >
build_relative_scope(base_build  &build, relative_scope_ast const &ast) {
    auto s{ast.get_father_scope().build(build)};
    return s->qualified_scope_lookup(ast.get_name());
}
reference_ptr<base_build >
build_root_scope(base_build  &build, root_scope_ast const &ast) {
    reference_ptr<base_build >s{&build};
    while (s->m_father != nullptr) {
        s = s->m_father;
    }
    return s;
}
}