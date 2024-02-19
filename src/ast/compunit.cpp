#include "compunit.h"
void pache::compunit_ast::print() const {
  std::cout << "compunit_ast:\n"
            << "m_packages:{\n";
  for (auto const &ast : m_packages) {
    ast->print();
  }
  std::cout << "}\n"
            << "m_var_table:{\n";
  for (auto const &ast : m_var_table) {
    ast->print();
  }
  std::cout << "}\n"
            << "class_def:{\n";
  for (auto const &ast : class_def) {
    ast->print();
  }
  std::cout << "}\n"
            << "m_func_asts:{\n";
  for (auto const &ast : m_func_asts) {
    ast->print();
  }
  std::cout << "}\n";
}
