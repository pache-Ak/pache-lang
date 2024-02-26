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
literal_ast::literal_ast(std::string &&l, std::string &&s)
    : m_literal(std::move(l)), m_suffix(std::move(s)) {}
binary_integer_literal::binary_integer_literal(std::string &&l, std::string &&s)
    : literal_ast(std::move(l), std::move(s)) {}
binary_integer_literal::binary_integer_literal(std::string &&l)
    : literal_ast(std::move(l), "i32") {}
octal_integer_literal::octal_integer_literal(std::string &&l, std::string &&s)
    : literal_ast(std::move(l), std::move(s)) {}
octal_integer_literal::octal_integer_literal(std::string &&l)
    : literal_ast(std::move(l), "i32") {}
decimal_integer_literal::decimal_integer_literal(std::string &&l,
                                                 std::string &&s)
    : literal_ast(std::move(l), std::move(s)) {}
decimal_integer_literal::decimal_integer_literal(std::string &&l)
    : literal_ast(std::move(l), "i32") {}
hexadecimal_integer_literal::hexadecimal_integer_literal(std::string &&l,
                                                         std::string &&s)
    : literal_ast(std::move(l), std::move(s)) {}
hexadecimal_integer_literal::hexadecimal_integer_literal(std::string &&l)
    : literal_ast(std::move(l), "i32") {}
} // namespace pache
