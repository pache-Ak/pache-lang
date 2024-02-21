#include "class.h"
#include <iostream>
#include <utility>

pache::class_ast::class_ast(std::string && name, class_body &&body)
    : m_name(std::move(name)), m_body(std::move(body)) {
}
void pache::class_ast::print() const {
  std::cout << "class_ast:\n"
            << "m_name:" << m_name << "\n"
            << "inner_class_def:{\n";
  for (auto const &ast : m_body.inner_class_def) {
    ast->print();
  }
  std::cout << "}\n"
            << "var_def:{\n";
  for (auto const &ast : m_body.var_def) {
    ast->print();
  }
  std::cout << "}\n"
            << "func_def:{\n";
  for (auto const &ast : m_body.func_def) {
    ast->print();
  }
  std::cout << "}\n";
}
