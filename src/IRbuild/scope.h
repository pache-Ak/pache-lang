#ifndef SCOPE_H
#define SCOPE_H

#include "../reference_ptr.h"
#include "build.h"
#include "../ast/scope.h"

namespace pache {
reference_ptr<base_build >
build_unqualified_scope(base_build  &build, unqualified_scope_ast const &ast);
reference_ptr<base_build >
build_relative_scope(base_build  &build, relative_scope_ast const &ast);
reference_ptr<base_build >
build_root_scope(base_build  &build, root_scope_ast const &ast);
}

#endif
