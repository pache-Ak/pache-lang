#ifndef IR_STATEMNET_H
#define IR_STATEMNET_H

#include "../ast/statement.h"
#include "build.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Instructions.h"

#include <memory>
#include <utility>
#include <vector>

namespace pache {

class block_scope : public base_build {
public:
  explicit block_scope(base_build *const father)
      : base_build(father, ""), has_terminator(father) {}
      
   virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const override {
    // should not be call
    return {};
   }

  virtual void insert(std::string_view name,
                      std::unique_ptr<build_variable> &&value) ;
  void deallco_all();
  ~block_scope() { deallco_all(); }
  virtual llvm::BasicBlock *get_loop_begin() const;
  virtual llvm::BasicBlock *get_loop_end() const ;

  virtual reference_ptr<build_variable>
  qualified_var_lookup(std::string_view name) override;
  virtual reference_ptr<build_type const>
  qualified_type_lookup(std::string_view name) const override;
  virtual reference_ptr<base_build>
  qualified_scope_lookup(std::string_view name) override;

  virtual bool is_block() const {
    return true;
  }

  
  bool has_terminator;
private:
  // here need a symbol table
  // search by string
  // Need to maintain the order of insertion
  // Because it needs to be destroyed in reverse order of insertion
  std::vector<std::pair<std::string_view, std::unique_ptr<build_variable>>>
      named_values;
};
void statement_build(block_scope &father, stmt_ast const &ast);

void assign_stmt_build(block_scope &father, assign_stmt const &ast);

void return_void_stmt_build(block_scope &father, return_void_stmt const &ast);

void return_exp_stmt_build(block_scope &father, return_ast const &ast);

void block_build(block_scope &father, block_ast const &ast);

void build_loop(block_scope &father, block_ast const &ast);

void build_if(block_scope &father, if_stmt const &ast);
void build_if_else(block_scope &father, if_else_stmt const &ast);

void build_break(block_scope &father, break_stmt const &ast);
void build_continue(block_scope &father, continue_stmt const &ast);
void build_let(block_scope &father, let_stmt const &ast);
void build_assign(base_build &father, assign_stmt const &ast);
void build_exp_stmt(base_build &father, exp_stmt const &ast);
} // namespace pache

#endif
