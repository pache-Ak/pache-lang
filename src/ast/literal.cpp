#include "literal.h"
#include "IRbuild/literal.h"

namespace pache {
literal_ast::~literal_ast(){}

std::unique_ptr<build_variable>
binary_integer_literal::build(base_build &build) const {
  return build_integer_literal(build, *this);
}

std::unique_ptr<build_variable>
octal_integer_literal::build(base_build &build) const {
  return build_integer_literal(build, *this);
}

std::unique_ptr<build_variable>
decimal_integer_literal::build(base_build &build) const {
  return build_integer_literal(build, *this);
}

std::unique_ptr<build_variable>
hexadecimal_integer_literal::build(base_build &build) const {
  return build_integer_literal(build, *this);
}
} // namespace pache
