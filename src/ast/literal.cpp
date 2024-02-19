#include "literal.h"
#include "IRbuild/literal.h"
#include <iostream>
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
void binary_integer_literal::print() const {
  std::cout << "binary_integer_literal:\n"
            << "m_literal:\t" << m_literal << "\n"
            << "m_suffix:\t" << m_suffix << "\n";
}
void octal_integer_literal::print() const {
  std::cout << "octal_integer_literal:\n"
            << "m_literal:\t" << m_literal << "\n"
            << "m_suffix:\t" << m_suffix << "\n";
}
void decimal_integer_literal::print() const {
  std::cout << "decimal_integer_literal:\n"
            << "m_literal:\t" << m_literal << "\n"
            << "m_suffix:\t" << m_suffix << "\n";
}
void hexadecimal_integer_literal::print() const {
  std::cout << "hexadecimal_integer_literal:\n"
            << "m_literal:\t" << m_literal << "\n"
            << "m_suffix:\t" << m_suffix << "\n";
}
} // namespace pache
