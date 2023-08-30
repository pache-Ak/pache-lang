#ifndef IR_STATEMNET_H
#define IR_STATEMNET_H

#include "../ast/ast.h"
#include "../ast/statement.h"
#include "build.h"
#include "type.h"
#include "llvm/IR/Instructions.h"

#include <memory>
#include <memory_resource>
#include <string_view>
#include <unordered_map>
#include <utility>
#include <vector>

namespace pache {

void statement_build(base_build *father, std::unique_ptr<stmt_ast> &ast) {
  ast->build(father);
}

class block_build : public base_build {
public:
  explicit block_build(base_build *father,
                       std::unique_ptr<block_ast> const &ast)
      : base_build(father) {
    for (std::unique_ptr<stmt_ast> &stmt : ast->get_stmt_list()) {
      statement_build(this, stmt);
    }
  }
  virtual llvm::Value *find_var(std::string const &name) const override {
    auto it = named_values.begin();
    for (; it != named_values.end(); ++it) {
      if (it->first == name) {
        break;
      }
    }

    if (it != named_values.end()) {
      return it->second.get_value();
    } else if (m_father != nullptr) {
      return m_father->find_var(name);
    } else {
      std::cerr << "file_name: line: "
                << "name " << name << "not found!\n";
      return nullptr;
    }
  }
  void build(block_ast *ast);
  virtual void insert(std::string const &name,
                      build_variable &&value) override {
    if (find_var(name) == nullptr) {
      named_values.emplace_back(name, value);
    } else {
      // TODO log error as this:
      std::cerr << "file_name: line: "
                << "redifintion " << name << "\n";
    }
  }
  void deallco_all() {
    for (auto beg = named_values.rbegin(); beg != named_values.rend(); ++beg) {
      // find deallocate funcion
      // IR::Builder->CreateCall(/* function callee */,
      // {beg->second->get_value()},
      // /*destory function name */ );
    }
  }
  ~block_build() { deallco_all(); }

private:
  // here need a symbol table
  // search by string
  // Need to maintain the order of insertion
  // Because it needs to be destroyed in reverse order of insertion
  std::vector<std::pair<std::string_view, build_variable>> named_values;
};

void build_block(base_build *father, std::unique_ptr<block_ast> const &ast) {
  block_build b{father, ast};
}

void build_loop(base_build *father, std::unique_ptr<block_ast> const &ast) {
  llvm::BasicBlock *entryBB = llvm::BasicBlock::Create(
      *IR::TheContext, "loop", IR::Builder->GetInsertBlock()->getParent());
  llvm::BasicBlock *endBB = llvm::BasicBlock::Create(
      *IR::TheContext, "endEntry", IR::Builder->GetInsertBlock()->getParent());

  IR::Builder->SetInsertPoint(entryBB);
  block_build b(father, ast);
  // IR alloc will run only once
  // need TODO
  // constuct function need run per loop

  b.deallco_all();

  IR::Builder->CreateBr(entryBB);
}

void build_if(base_build *father, block_ast *ast) {}
} // namespace pache

#endif
