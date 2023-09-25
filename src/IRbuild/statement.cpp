#include "statement.h"
#include "build.h"
#include "expression.h"
#include "type.h"
#include "variable.h"
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

namespace pache {

void statement_build(block_scope *father, stmt_ast const *const ast) {
  ast->build(father);
}

void assign_stmt_build(block_scope *father, assign_stmt const *const ast) {
  llvm::Value *var = father->find_var(ast->get_var()->get_name())->get_value();
  if (var == nullptr) {
    // TODO log error
    // maybe like this
    // std::cerr << "Unknown variable name.\n";
    return;
  }

  llvm::Value *val = build_exp(father, ast->get_exp());
  if (val == nullptr) {
    // TODO log error
    // maybe like this
    // std::cerr << "expression is not vaild.\n";
    return;
  }

  Builder->CreateStore(val, var);
}

void return_void_stmt_build(block_scope *father,
                            return_void_stmt const *const ast) {
  Builder->CreateRetVoid();
}

void return_exp_stmt_build(block_scope *father, return_ast const *const ast) {
  llvm::Value *val = build_exp(father, ast->get_exp());
  if (val == nullptr) {
    // TODO log error
    // maybe like this
    // std::cerr << "expression is not vaild.\n";
    return;
  }

  Builder->CreateRet(val);
}

std::unique_ptr<build_variable> const &
block_scope::find_var(std::string const &name) const {
  for (auto it = named_values.begin(); it != named_values.end(); ++it) {
    if (it->first == name) {
      return it->second;
    }
  }

  if (base_build::m_father != nullptr) {
    return base_build::m_father->find_var(name);
  } else {
    std::cerr << "file_name: line: "
              << "name " << name << "not found!\n";
    return nullptr;
  }
}
build_type *const block_scope::find_type(std::string const &name) const {
  if (base_build::m_father != nullptr) {
    return base_build::m_father->find_type(name);
  } else {
    return nullptr;
  }
}

void block_scope::insert(std::string &&name,
                         std::unique_ptr<build_variable> &&value) {
  if (find_var(name) == nullptr) {
    named_values.emplace_back(
        std::make_pair(std::move(name), std::move(value)));
  } else {
    // TODO log error as this:
    std::cerr << "file_name: line: "
              << "redifintion " << name << "\n";
  }
}
void block_scope::insert(std::string const &name,
                         std::unique_ptr<build_variable> &&value) {
  if (find_var(name) == nullptr) {
    named_values.emplace_back(name, std::move(value));
  } else {
    // TODO log error as this:
    std::cerr << "file_name: line: "
              << "redifintion " << name << "\n";
  }
}
void block_scope::deallco_all() {
  for (auto beg = named_values.rbegin(); beg != named_values.rend(); ++beg) {
    // find deallocate funcion
    // IR::Builder->CreateCall(/* function callee */,
    // {beg->second->get_value()},
    // /*destory function name */ );
  }
}

void block_build(block_scope *father, block_ast const *const ast) {
  std::unique_ptr<block_scope> scope{
      std::make_unique<block_scope>(father, father->get_loop_father())};

  for (auto &stmt : ast->get_stmt_list()) {
    statement_build(scope.get(), stmt.get());
  }
}

class loop_scope : public block_scope {
public:
  explicit loop_scope(block_scope *father, loop_label const *const loop,
                      llvm::BasicBlock *begin, llvm::BasicBlock *end)
      : block_scope(father, loop), loop_begin(begin), loop_end(end) {}

  virtual llvm::BasicBlock *get_loop_begin() const override {
    return loop_begin;
  }
  virtual llvm::BasicBlock *get_loop_end() const override { return loop_end; }

private:
  llvm::BasicBlock *loop_begin;
  llvm::BasicBlock *loop_end;
};

void build_loop(block_scope *const father, block_ast const *const ast) {
  llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(
      *IR::TheContext, "loop", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *endBB = llvm::BasicBlock::Create(
      *IR::TheContext, "endEntry", IR::Builder->GetInsertBlock()->getParent());

  IR::Builder->SetInsertPoint(entryBB);
  std::unique_ptr<loop_scope> scope{std::make_unique<loop_scope>(
      father, father->get_loop_father(), entryBB, endBB)};
  for (auto &stmt : ast->get_stmt_list()) {
    statement_build(scope.get(), stmt.get());
  }
  scope->deallco_all();
  IR::Builder->CreateBr(entryBB);
}

void build_break(block_scope *const father, break_stmt const *const ast) {

  if (father->get_loop_end() != nullptr) {
    IR::Builder->CreateBr(father->get_loop_end());
  } else {
    std::cerr << "break not in loop.\n";
  }
}
void build_continue(block_scope *const father, continue_stmt const *const ast) {
  if (father->get_loop_begin() != nullptr) {
    IR::Builder->CreateBr(father->get_loop_begin());
  } else {
    std::cerr << "break not in loop.\n";
  }
}

void build_if(block_scope *const father, if_stmt const *const ast) {
  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge", IR::Builder->GetInsertBlock()->getParent());

  llvm::Value *condition = build_exp(father, ast->get_condition());
  // need implicit cast to boolean
  Builder->CreateCondBr(condition, then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast->get_then_block());
  Builder->CreateBr(merge_BB);
}

void build_if_else(block_scope *const father, if_else_stmt const *const ast) {
  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *else_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "else", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge", IR::Builder->GetInsertBlock()->getParent());

  llvm::Value *condition = build_exp(father, ast->get_condition());
  // need implicit cast to boolean
  Builder->CreateCondBr(condition, then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast->get_then_block());
  Builder->CreateBr(merge_BB);

  Builder->SetInsertPoint(else_BB);
  statement_build(father, ast->get_else_block());
  Builder->CreateBr(merge_BB);
}

void build_let(block_scope *const father, let_stmt const *const ast) {
  if (dynamic_cast<block_scope *const>(father) == nullptr) {
    std::cerr << "can't define variable in here.\n";
    return;
  }
  std::unique_ptr<build_type> type{type_build(father, ast->get_var_type())};
  llvm::AllocaInst *all = IR::Builder->CreateAlloca(
      type->get_llvm_type(), nullptr, ast->get_var_name());

  father->insert(ast->get_var_name(),
                 std::make_unique<build_local_variable>(std::move(type), all));

  if (ast->get_init_exp() != nullptr) {
    auto exp = build_exp(father, ast->get_init_exp().get());
    IR::Builder->CreateStore(exp, all);
  } else {
    std::cerr << "define variable without initializer.\n";
  }
}

void build_assign(base_build *const father, assign_stmt const *const ast) {
  std::unique_ptr<build_variable> const &var =
      father->find_var(ast->get_var()->get_name());
  if (var == nullptr) {
    std::cerr << "variable hasn't defined.\n";
  } else {
    auto exp = build_exp(father, ast->get_exp());
    if (exp == nullptr) {
      std::cerr << "expression is not valid.\n";
    } else {
      IR::Builder->CreateStore(exp, var->get_value());
    }
  }
}

void build_exp_stmt(base_build *const father, exp_stmt const *const ast) {
  build_exp(father, ast->get_exp());
}
} // namespace pache