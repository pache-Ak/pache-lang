#include "../IRbuild/statement.h"
#include "statement.h"
#include <memory>

void pache::block_ast::build(block_scope *const father) const {
  block_build(father, this);
}

void pache::loop_stmt::build(block_scope *const father) const {
  build_loop(father, this);
}

void pache::if_stmt::build(block_scope *const father) const {
  build_if(father, this);
}

void pache::if_else_stmt::build(block_scope *const father) const {
  build_if_else(father, this);
}

void pache::return_void_stmt::build(block_scope *const father) const {
  return_void_stmt_build(father, this);
}

void pache::return_ast::build(block_scope *const father) const {
  return_exp_stmt_build(father, this);
}

void pache::break_stmt::build(block_scope *const father) const {
  build_break(father, this);
}

void pache::continue_stmt::build(block_scope *const father) const {
  build_continue(father, this);
}

void pache::let_stmt::build(block_scope *const father) const {
  build_let(father, this);
}

void pache::assign_stmt::build(block_scope *const father) const {
  build_assign(father, this);
}

void pache::exp_stmt::build(block_scope *const father) const {
  build_exp_stmt(father, this);
}