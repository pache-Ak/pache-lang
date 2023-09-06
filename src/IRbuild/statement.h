#ifndef IR_STATEMNET_H
#define IR_STATEMNET_H

#include "../ast/ast.h"
#include "../ast/statement.h"
#include "build.h"
#include "expression.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Instructions.h"

#include <memory>
#include <memory_resource>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pache {

void statement_build(base_build *father, stmt_ast const *const ast);

void assign_stmt_build(base_build *father, assign_stmt const *const ast);

void return_void_stmt_build(base_build *father,
                            return_void_stmt const *const ast);

void return_exp_stmt_build(base_build *father, return_ast const *const ast);

void block_build(base_build *father, block_ast const *const ast);

void build_loop(base_build *const father, block_ast const *const ast);

void build_if(base_build *const father, if_stmt const *const ast);
void build_if_else(base_build *const father, if_else_stmt const *const ast);

void build_break(base_build *const father, break_stmt const *const ast);
void build_continue(base_build *const father, continue_stmt const *const ast);
void build_let(base_build *const father, let_stmt const *const ast);
void build_assign(base_build *const father, assign_stmt const *const ast);
void build_exp_stmt(base_build *const father, exp_stmt const *const ast);
} // namespace pache

#endif
