#include "statement.h"
#include "build.h"
#include "expression.h"
#include "reference_ptr.h"
#include "type.h"
#include "variable.h"
#include <iostream>
#include <llvm/IR/BasicBlock.h>
#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Instructions.h>
#include <llvm/IR/Value.h>
#include <memory>
#include <utility>

namespace pache {

void statement_build(block_scope &father, stmt_ast const &ast) {
  ast.build(father);
}

void assign_stmt_build(block_scope &father, assign_stmt const &ast) {
  if (auto var =build_expression(father, ast.get_var()) ; var != nullptr) {
    // TODO log error
    // maybe like this
    // std::cerr << "Unknown variable name.\n";
    return;
  }else {

    llvm::Value *val = ast.get_exp().build(father)->get_value();
    if (val == nullptr) {
      // TODO log error
      // maybe like this
      // std::cerr << "expression is not vaild.\n";
      return;
    }

    Builder->CreateStore(val, var->get_value());

  
  }
  
    
}

void return_void_stmt_build(block_scope &father, return_void_stmt const &ast) {
  Builder->CreateRetVoid();
}

void return_exp_stmt_build(block_scope &father, return_ast const &ast) {
  llvm::Value *val = ast.get_exp()->build(father)->get_value();
  if (val == nullptr) {
    // TODO log error
    // maybe like this
    // std::cerr << "expression is not vaild.\n";
    return;
  }

  Builder->CreateRet(val);
}


void block_scope::insert(std::string_view name,
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

void block_build(block_scope &father, block_ast const &ast) {
  std::unique_ptr<block_scope> scope{
      std::make_unique<block_scope>(&father)};

  for (auto &stmt : ast.get_stmt_list()) {
    statement_build(*scope, *stmt);
  }
}

class loop_scope : public block_scope {
public:
  explicit loop_scope(block_scope * const father,
                      llvm::BasicBlock *begin, llvm::BasicBlock *end)
      : block_scope(father), loop_begin(begin), loop_end(end) {}

  virtual llvm::BasicBlock *get_loop_begin() const override {
    return loop_begin;
  }
  virtual llvm::BasicBlock *get_loop_end() const override { return loop_end; }

private:
  llvm::BasicBlock *loop_begin;
  llvm::BasicBlock *loop_end;
};

void build_loop(block_scope &father, block_ast const &ast) {
  llvm::Function * the_function =  Builder->GetInsertBlock()->getParent();
  llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(
      *IR::TheContext, "loop", the_function);
  llvm::BasicBlock *endBB = llvm::BasicBlock::Create(
      *IR::TheContext, "endEntry");

  IR::Builder->SetInsertPoint(entryBB);
  std::unique_ptr<loop_scope> scope{std::make_unique<loop_scope>(
      &father, entryBB, endBB)};
  for (auto &stmt : ast.get_stmt_list()) {
    statement_build(*scope, *stmt);
  }
  scope->deallco_all();
  IR::Builder->CreateBr(entryBB);
  the_function->insert(the_function->end(), endBB);
  Builder->SetInsertPoint(endBB);
}

void build_break(block_scope & father, break_stmt const &ast) {

  if (father.get_loop_end() != nullptr) {
    IR::Builder->CreateBr(father.get_loop_end());
  } else {
    std::cerr << "break not in loop.\n";
  }
}
void build_continue(block_scope &father, continue_stmt const &ast) {
  if (father.get_loop_begin() != nullptr) {
    IR::Builder->CreateBr(father.get_loop_begin());
  } else {
    std::cerr << "break not in loop.\n";
  }
}

void build_if(block_scope &father, if_stmt const &ast) {
  auto condition = build_expression(father, ast.get_condition())->get_value();
  llvm::Function * the_function =  Builder->GetInsertBlock()->getParent();
  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", the_function);
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge");

  // need implicit cast to boolean
  Builder->CreateCondBr(condition, then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast.get_then_block());
  Builder->CreateBr(merge_BB);

  then_BB = Builder->GetInsertBlock();
  the_function->insert(the_function->end(), merge_BB);
  Builder->SetInsertPoint(merge_BB);
}

void build_if_else(block_scope &father, if_else_stmt const &ast) {
  auto condition = build_expression(father, ast.get_condition());
  llvm::Function * the_function =  Builder->GetInsertBlock()->getParent();

  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", the_function);
  llvm::BasicBlock *else_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "else");
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge");

  // need implicit cast to boolean
  Builder->CreateCondBr(condition->get_value(), then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast.get_then_block());
  Builder->CreateBr(merge_BB);

  then_BB = Builder->GetInsertBlock();
  the_function->insert(the_function->end(), else_BB);
  Builder->SetInsertPoint(else_BB);
  statement_build(father, ast.get_else_block());
  Builder->CreateBr(merge_BB);

  else_BB = Builder->GetInsertBlock();
  the_function->insert(the_function->end(), merge_BB);
  Builder->SetInsertPoint(merge_BB);
}

void build_let(block_scope &father, let_stmt const &ast) {
  // if (dynamic_cast<block_scope *const>(father) == nullptr) {
  //   std::cerr << "can't define variable in here.\n";
  //   return;
  // }
  std::unique_ptr<build_type> type{type_build(father, ast.get_var_type())};
  if (type == nullptr) {
    // log error
    return;
  }
  llvm::AllocaInst *all = IR::Builder->CreateAlloca(
      type->get_llvm_type(), nullptr, ast.get_var_name());

  if (type->is_reference()) {
    father.insert(ast.get_var_name(), std::make_unique<build_local_reference>(
                                            std::move(type), all));
  } else {
    father.insert(ast.get_var_name(), std::make_unique<build_local_variable>(
                                            std::move(type), all));
  }

  if (std::unique_ptr<build_variable> exp;
      ast.get_init_exp() != nullptr &&
      (exp = build_expression(father,*ast.get_init_exp().get())) != nullptr) {
    IR::Builder->CreateStore(
        exp->get_value(), father.find_var(ast.get_var_name())->get_value());
  } else {
    std::cerr << "define variable without initializer.\n";
  }
}

void build_assign(base_build &father, assign_stmt const &ast) {
  std::unique_ptr<build_variable> const &var =
      build_expression(father, ast.get_var());
  if (var == nullptr) {
    std::cerr << "variable hasn't defined.\n";
  } else {
    auto exp = build_expression(father, ast.get_exp());
    if (exp == nullptr) {
      std::cerr << "expression is not valid.\n";
    } else {
      IR::Builder->CreateStore(exp->get_value(), var->get_value());
    }
  }
}

void build_exp_stmt(base_build &father, exp_stmt const &ast) {
  build_expression(father, ast.get_exp());
}
llvm::BasicBlock *block_scope::get_loop_begin() const {
  if (m_father == nullptr) {
    return nullptr;
  } else if (m_father->is_block()){
    return static_cast<block_scope*>(m_father)->get_loop_begin();
  }
}
llvm::BasicBlock *block_scope::get_loop_end() const {
  if (m_father == nullptr) {
    return nullptr;
  } else if (m_father->is_block()){
    return static_cast<block_scope*>(m_father)->get_loop_end();
  }
}
reference_ptr<build_variable>
block_scope::qualified_var_lookup(std::string_view name) {
  if (auto it = std::find_if(
          named_values.begin(), named_values.end(),
          [name](
              std::pair<std::string_view, std::unique_ptr<build_variable>> const
                  &arg) { return arg.first == name; });
      it != named_values.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}
reference_ptr<build_type const>
block_scope::qualified_type_lookup(std::string_view name) const {
  return nullptr;
}
reference_ptr<base_build>
block_scope::qualified_scope_lookup(std::string_view name) {
  return nullptr;
}
} // namespace pache