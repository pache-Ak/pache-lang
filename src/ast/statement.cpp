#include "../IRbuild/statement.h"
#include "statement.h"
#include <memory>

void pache::block_ast::build(base_build *const father) const {
  block_build(father, this);
}

void pache::loop_stmt::build(base_build *const father) const {
  build_loop(father, this);
}

void pache::if_stmt::build(base_build *const father) const {
  build_if(father, this);
}

void pache::if_else_stmt::build(base_build *const father) const {
  build_if_else(father, this);
}

void pache::return_void_stmt::build(base_build *const father) const {
  return_void_stmt_build(father, this);
}

void pache::return_ast::build(base_build *const father) const {
  return_exp_stmt_build(father, this);
}

void pache::break_stmt::build(base_build *const father) const {
  build_break(father, this);
}

void pache::continue_stmt::build(base_build *const father) const {
  build_continue(father, this);
}