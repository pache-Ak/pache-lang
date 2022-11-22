#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "statement.h"

namespace pache {
  class compunit_ast : public base_ast {
  public:
    virtual std::string dump() override {
      std::cout << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
      std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

      for (auto  func : m_func_table) {
        func->dump();
      }
      return "";
    }
    virtual void insert_dec(stmt_ast *let) override {
      let->set_father(this);
      m_var_table.push_back(std::unique_ptr<stmt_ast>{let});
    }

    virtual void insert_dec(variable_ast *func) override {
      m_func_table.push_back(func);
      func->set_father(this);
    }
  private:
    std::vector<variable_ast*> m_func_table;
    std::vector<std::unique_ptr<stmt_ast>> m_var_table;
  };

}

#endif