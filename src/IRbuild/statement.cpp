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
reference_ptr<build_variable>
block_scope::find_var(std::string_view name) const {
  // 首先会查找包含这次名字使用的块中这次使用之前的部分
  for (auto &it: named_values) {
    if (it.first == name) {
      return it.second;
    }
  }

  if (base_build::m_father != nullptr) {
    // 然后查找外围块在该块开头之前的部分
    return base_build::m_father->find_var(name);
  } else {
    // TODO
    // block should be in function 
    // base_build::m_father shouldn't be null
    std::cerr << "file_name: line: "
              << "name " << name << "not found!\n";
    return nullptr;
  }
}

reference_ptr<build_type const>
block_scope::find_type(std::string_view name) const {
  if (base_build::m_father != nullptr) {
    return base_build::m_father->find_type(name);
  } else {
    // TODO log error
    return nullptr;
  }
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
      std::make_unique<block_scope>(&father, father.get_loop_father())};

  for (auto &stmt : ast.get_stmt_list()) {
    statement_build(*scope, *stmt);
  }
}

class loop_scope : public block_scope {
public:
  explicit loop_scope(block_scope &father, loop_label const *const loop,
                      llvm::BasicBlock *begin, llvm::BasicBlock *end)
      : block_scope(&father, loop), loop_begin(begin), loop_end(end) {}

  virtual llvm::BasicBlock *get_loop_begin() const override {
    return loop_begin;
  }
  virtual llvm::BasicBlock *get_loop_end() const override { return loop_end; }

private:
  llvm::BasicBlock *loop_begin;
  llvm::BasicBlock *loop_end;
};

void build_loop(block_scope &father, block_ast const &ast) {
  llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(
      *IR::TheContext, "loop", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *endBB = llvm::BasicBlock::Create(
      *IR::TheContext, "endEntry", IR::Builder->GetInsertBlock()->getParent());

  IR::Builder->SetInsertPoint(entryBB);
  std::unique_ptr<loop_scope> scope{std::make_unique<loop_scope>(
      father, father.get_loop_father(), entryBB, endBB)};
  for (auto &stmt : ast.get_stmt_list()) {
    statement_build(*scope, *stmt);
  }
  scope->deallco_all();
  IR::Builder->CreateBr(entryBB);
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
  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge", IR::Builder->GetInsertBlock()->getParent());

  auto condition = build_expression(father, ast.get_condition())->get_value();
  // need implicit cast to boolean
  Builder->CreateCondBr(condition, then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast.get_then_block());
  Builder->CreateBr(merge_BB);
}

void build_if_else(block_scope &father, if_else_stmt const &ast) {
  llvm::BasicBlock *then_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "then", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *else_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "else", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *merge_BB = llvm::BasicBlock::Create(
      *IR::TheContext, "merge", IR::Builder->GetInsertBlock()->getParent());

  auto condition = build_expression(father, ast.get_condition());
  // need implicit cast to boolean
  Builder->CreateCondBr(condition->get_value(), then_BB, merge_BB);

  Builder->SetInsertPoint(then_BB);
  block_build(father, ast.get_then_block());
  Builder->CreateBr(merge_BB);

  Builder->SetInsertPoint(else_BB);
  statement_build(father, ast.get_else_block());
  Builder->CreateBr(merge_BB);
}

void build_let(block_scope &father, let_stmt const &ast) {
  // if (dynamic_cast<block_scope *const>(father) == nullptr) {
  //   std::cerr << "can't define variable in here.\n";
  //   return;
  // }
  std::unique_ptr<build_type> type{type_build(father, ast.get_var_type())};
  llvm::AllocaInst *all = IR::Builder->CreateAlloca(
      type->get_llvm_type(), nullptr, ast.get_var_name());

  if (type->is_reference()) {
    father.insert(ast.get_var_name(), std::make_unique<build_local_reference>(
                                            std::move(type), all));
  } else {
    father.insert(ast.get_var_name(), std::make_unique<build_local_variable>(
                                            std::move(type), all));
  }

  if (ast.get_init_exp() != nullptr) {
    auto exp = build_expression(father,*ast.get_init_exp().get());
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
llvm::BasicBlock *loop_label::get_loop_begin() const {
  if (m_father != nullptr) {
    return m_father->get_loop_begin();
  } else {
    return nullptr;
  }
}
llvm::BasicBlock *loop_label::get_loop_end() const {
  if (m_father != nullptr) {
    return m_father->get_loop_end();
  } else {
    return nullptr;
  }
}
llvm::BasicBlock *block_scope::get_loop_begin() const {
  if (loop_label::m_father != nullptr) {
    return loop_label::m_father->get_loop_begin();
  } else {
    return nullptr;
  }
}
llvm::BasicBlock *block_scope::get_loop_end() const {
  if (loop_label::m_father != nullptr) {
    return loop_label::m_father->get_loop_end();
  } else {
    return nullptr;
  }
}
loop_label const *const block_scope::get_loop_father() const {
  return loop_label::m_father;
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