#include "scope.h"
#include "ast/scope.h"
#include <memory>

namespace pache {
build_scope::~build_scope(){}

std::unique_ptr<build_scope>
build_unqualified_scope(base_build  &build, unqualified_scope_ast const &ast) {
    return std::make_unique<unqualified_scope>(&build);
}
std::unique_ptr<build_scope>
build_relative_scope(base_build  &build, relative_scope_ast const &ast) {
    auto s{ast.get_father_scope().build(build)};
    return std::make_unique<qualified_scope>(s->find_scope(ast.get_name()));
}
std::unique_ptr<build_scope>
build_root_scope(base_build  &build, root_scope_ast const &ast) {
    reference_ptr<base_build >s{&build};
    while (s->m_father != nullptr) {
        s = s->m_father;
    }
    return std::make_unique<qualified_scope>(std::move(s));
}
}