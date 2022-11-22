#ifndef AST_H
#define AST_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <unordered_map>
#include "type.h"

namespace pache {
  inline namespace {
    std::size_t ssa_value = 0;
  }

  class variable_ast;
  class func_ast;

  class base_ast {
  public:
    virtual ~base_ast() = default;
    virtual std::string dump() = 0;
    void set_father(base_ast *father) {
      m_father = father;
      return;
    }
    virtual void insert_dec(variable_ast *dec ) { };
    virtual void insert_dec(std::string name, variable_ast * dec)  {
      m_father->insert_dec(name, dec);
    }

    virtual variable_ast * find_dec(std::string name) const {
      return m_father->find_dec(name);
    }
    virtual std::string begin_lable() const {
      if (m_father != nullptr) {
        return m_father->begin_lable();
      } else {
        std::cout << "error : not in loop block.\n";
        return "";
      }
    }

    virtual std::string end_lable() const {
      if (m_father != nullptr) {
        return m_father->end_lable();
      } else {
        std::cout << "error : not in loop block.\n";
        return "";
      }
    }
    std::string get_prefix() {
      return name_prefix;
    }

    void set_prefix(std::string pre) {
      name_prefix = pre;
    }

    std::size_t next_block_value() {
      return block_value++;
    }
  protected:
    base_ast *m_father = nullptr;
    std::unordered_map<std::string, type_ast*> type_name;
    std::string name_prefix;
    std::size_t block_value = 0;
  };
  class variable_ast : public base_ast {
  public:
    explicit variable_ast(const type_ast *type, std::string *name)
      : m_type(type), real_name(name) { }

    const type_ast * get_type() {
      return m_type;
    }

    std::string *get_name() {
      return real_name;
    }

    void set_name(std::string name) {
      *real_name = name;
    }
    virtual std::string dump() override {
      return "";
    }
  protected:
    const type_ast *m_type;
    std::string *real_name;
  };







// ...
    class func_arg : public variable_ast {
  public:
    explicit func_arg(const type_ast *type, std::string *str) : variable_ast(type, str) { }
    virtual std::string dump() {
      return get_type()->dump() + "%" + *get_name();
    }

  private:
  };


  class func_ast : public variable_ast {
  public:
    explicit func_ast(const type_ast *type, std::string *name, std::vector<func_arg*> *args)
      : variable_ast(type, name), m_args(args) { }
    std::unique_ptr<base_ast> block;

    virtual variable_ast* find_dec(std::string name) const override {
      auto beg = std::find_if(m_args->begin(), m_args->end(),  [=](func_arg* arg){return *(arg->get_name()) == name;});
      if (beg != m_args->end()) {
        return *beg;
      } else {
        return m_father->find_dec(name);
      }
    }
    virtual std::string dump() override {
      std::cout << "define "
          << get_type()->dump()
          << " @" << *real_name << "(";
      for (auto arg : *m_args) {
        std::cout << arg->get_type()->dump()
                  << " %" << *(arg->get_name())
                  << ", ";
      }
      std::cout    << ") {"
          << block->dump()
          << "}\n";

      return std::string{};
    }
  private:
    std::unique_ptr<std::vector<func_arg*>> m_args;
  };
    class main_func_ast : public func_ast {
  public:
  explicit main_func_ast(const type_ast *type, std::string *name, std::vector<func_arg*> *args)
    : func_ast(type, name, args) { }
    std::unique_ptr<base_ast> block;
    virtual std::string dump() override {
      std::cout << "define "
          << "i32"
          << " @" << "main" << "("
    //  for (auto arg : m_args) {
      //  std::cout << arg->
      //}
          << ") "
          << block->dump();

      return std::string{};
    }


  };


  class compunit_ast : public base_ast {
  public:
    virtual std::string dump() override {
      std::cout << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
      std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

      for (auto  func : m_symbpol_table) {
        func->dump();
      }
      return "";
    }
    virtual void insert_dec(variable_ast *func) override {
      m_symbpol_table.push_back(func);
      func->set_father(this);
    }
  private:
    std::vector<variable_ast*> m_symbpol_table;
  };

}


#endif
