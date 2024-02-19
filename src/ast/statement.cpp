#include "statement.h"
#include "../IRbuild/statement.h"
#include <iostream>

namespace pache {
stmt_ast::~stmt_ast(){}
void block_ast::print() const {
  std::cout << "block_ast:\n"
            << "m_statements:{\n";

  for (auto const &ast : m_statements) {
    ast->print();
  }

  std::cout << "}\n";
}
void break_stmt::print() const { std::cout << "break_stmt\n"; }
void continue_stmt::print() const { std::cout << "continue_stmt\n"; }
void return_void_stmt::print() const { std::cout << "continue_stmt\n"; }
void return_ast::print() const {
  std::cout << "return_ast\n"
            << "m_exp:\n";
  m_exp->print();
}
void assign_stmt::print() const {
  std::cout << "assign_stmt\n"
            << "m_var:\n";
  m_var->print();
  std::cout << "m_val:\n";
  m_val->print();
}
void let_stmt::print() const {
  std::cout << "let_stmt\n"
            << "type:\n";
  type->print();
  std::cout << "var_name:\n" << var_name << "\n";
  std::cout << "m_init:\n";
  m_init->print();
}
void exp_stmt::print() const {
  std::cout << "exp_stmt\n"
            << "m_exp:\n";
  m_exp->print();
}
void if_stmt::print() const {
  std::cout << "if_stmt\n"
            << "m_condition:\n";
  m_condition->print();
  std::cout << "m_then_block:\n";
  m_then_block->print();
}
void if_else_stmt::print() const {
  std::cout << "if_else_stmt\n"
            << "m_condition:\n";
  m_condition->print();
  std::cout << "m_then_block:\n";
  m_then_block->print();
  std::cout << "m_else_block:\n";
  m_else_block->print();
}
} // namespace pache

void pache::block_ast::build(block_scope &father) const {
  block_build(father, *this);
}

void pache::loop_stmt::build(block_scope &father) const {
  build_loop(father, *this);
}

void pache::if_stmt::build(block_scope &father) const {
  build_if(father, *this);
}

void pache::if_else_stmt::build(block_scope &father) const {
  build_if_else(father, *this);
}

void pache::return_void_stmt::build(block_scope &father) const {
  return_void_stmt_build(father, *this);
}

void pache::return_ast::build(block_scope &father) const {
  return_exp_stmt_build(father, *this);
}

void pache::break_stmt::build(block_scope &father) const {
  build_break(father, *this);
}

void pache::continue_stmt::build(block_scope &father) const {
  build_continue(father, *this);
}

void pache::let_stmt::build(block_scope &father) const {
  build_let(father, *this);
}

void pache::assign_stmt::build(block_scope &father) const {
  build_assign(father, *this);
}

void pache::exp_stmt::build(block_scope &father) const {
  build_exp_stmt(father, *this);
}
pache::block_ast::block_ast(std::vector<std::unique_ptr<stmt_ast>> &&statements)
    : m_statements(std::move(statements)) {}
