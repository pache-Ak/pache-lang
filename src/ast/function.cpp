#include "function.h"

#include <iostream>
void pache::variable_ast::print() const {
  std::cout << "variable_ast:\n"
            << "m_type:\n";
  m_type->print();
  std::cout << "real_name:\n" << real_name << "\n";
}
void pache::func_arg::print() const {
  std::cout << "func_arg:\n"
            << "m_type:\n";
  m_type->print();
  std::cout << "real_name:\n" << real_name << "\n";
}
pache::type_ast const &pache::func_arg::get_type() const { return *m_type; }
std::string_view pache::func_arg::get_name() const { return real_name; }
void pache::func_ast::print() const {
  std::cout << "func_ast:\n"
            << "m_name:\n"
            << m_name << "\nm_args_name:{\n";
  for (auto const &ast : m_args_name) {
    std::cout << ast << "\n";
  }
  std::cout << "}\nm_type:\n";
  m_type.print();
  std::cout << "}m_block:{\n";
  for (auto const &stmt : m_block) {
    stmt->print();
  }
  std::cout << "}\n";
  
}
