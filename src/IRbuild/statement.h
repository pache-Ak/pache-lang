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
class loop_label {
public:
  explicit loop_label(loop_label const *const father) : m_father(father) {}
  virtual llvm::BasicBlock *get_loop_begin() const {
    if (m_father != nullptr) {
      return m_father->get_loop_begin();
    } else {
      return nullptr;
    }
  }
  virtual llvm::BasicBlock *get_loop_end() const {
    if (m_father != nullptr) {
      return m_father->get_loop_end();
    } else {
      return nullptr;
    }
  }

protected:
  loop_label const *const m_father;
};
class block_scope : public base_build, public loop_label {
public:
  explicit block_scope(base_build *father, loop_label const *const loop)
      : base_build(father), loop_label(loop) {}
  virtual std::unique_ptr<build_variable> const &
  find_var(std::string const &name) const override;

  virtual build_type *const find_type(std::string const &name) const override;
  virtual void insert(std::string &&name,
                      std::unique_ptr<build_variable> &&value) override;
  virtual void insert(std::string const &name,
                      std::unique_ptr<build_variable> &&value) override;
  void deallco_all();
  ~block_scope() { deallco_all(); }
  virtual llvm::BasicBlock *get_loop_begin() const override {
    if (loop_label::m_father != nullptr) {
      return loop_label::m_father->get_loop_begin();
    } else {
      return nullptr;
    }
  }
  llvm::BasicBlock *get_loop_end() const override {
    if (loop_label::m_father != nullptr) {
      return loop_label::m_father->get_loop_end();
    } else {
      return nullptr;
    }
  }
  loop_label const *const get_loop_father() const {
    return loop_label::m_father;
  }

private:
  // here need a symbol table
  // search by string
  // Need to maintain the order of insertion
  // Because it needs to be destroyed in reverse order of insertion
  std::vector<std::pair<std::string, std::unique_ptr<build_variable>>>
      named_values;
};
void statement_build(block_scope *father, stmt_ast const *const ast);

void assign_stmt_build(block_scope *father, assign_stmt const *const ast);

void return_void_stmt_build(block_scope *father,
                            return_void_stmt const *const ast);

void return_exp_stmt_build(block_scope *father, return_ast const *const ast);

void block_build(block_scope *father, block_ast const *const ast);

void build_loop(block_scope *const father, block_ast const *const ast);

void build_if(block_scope *const father, if_stmt const *const ast);
void build_if_else(block_scope *const father, if_else_stmt const *const ast);

void build_break(block_scope *const father, break_stmt const *const ast);
void build_continue(block_scope *const father, continue_stmt const *const ast);
void build_let(block_scope *const father, let_stmt const *const ast);
void build_assign(block_scope *const father, assign_stmt const *const ast);
void build_exp_stmt(block_scope *const father, exp_stmt const *const ast);
} // namespace pache

#endif
