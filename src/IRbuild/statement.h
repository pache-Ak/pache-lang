#ifndef IR_STATEMNET_H
#define IR_STATEMNET_H

#include "../ast/ast.h"
#include "../ast/statement.h"
#include "build.h"
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
class block_scope : public base_build {
public:
  explicit block_scope(base_build *father) : base_build(father) {}
  virtual build_variable *const
  find_var(std::string const &name) const override;

  virtual build_type *const find_type(std::string const &name) const override;
  virtual void insert(std::string const &name, build_variable &&value) override;
  void deallco_all();
  ~block_scope() { deallco_all(); }
  virtual llvm::BasicBlock *get_loop_begin() override {
    if (m_father != nullptr) {
      return m_father->get_loop_begin();
    } else {
      return nullptr;
    }
  }
  virtual llvm::BasicBlock *get_loop_end() override {
    if (m_father != nullptr) {
      return m_father->get_loop_end();
    } else {
      return nullptr;
    }
  }

private:
  // here need a symbol table
  // search by string
  // Need to maintain the order of insertion
  // Because it needs to be destroyed in reverse order of insertion
  std::vector<std::pair<std::string, build_variable *>> named_values;
};
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
