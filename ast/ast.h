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
#include <string>
#include <iostream>
#include <unordered_map>

namespace pache {
  inline namespace {
    std::size_t ssa_value = 0;
  }

  class variable_ast;
  class type_ast;

  class base_ast {
  public:
    virtual ~base_ast() = default;
    virtual std::string dump() = 0;
    void set_father(base_ast *father) {
      m_father = father;
      return;
    }

    virtual void insert_dec(std::string name, variable_ast * dec)  {
      m_father->insert_dec(name, dec);
    }

    virtual variable_ast * find_dec(std::string name) const {
      return m_father->find_dec(name);
    }
    virtual std::string begin_lable() const {
      return m_father->begin_lable();
    }

    virtual std::string end_lable() const {
      return m_father->end_lable();
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

  class compunit_ast : public base_ast {
  public:
    std::unique_ptr<base_ast> func_def;
    virtual std::string dump() override {
      std::cout << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
      std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

      return func_def->dump();
    }


  };

  class main_func_ast : public base_ast {
  public:
    std::unique_ptr<base_ast> block;
    virtual std::string dump() override {
      std::cout << "define "
          << "i32"
          << " @" << "main" << "("
      //  args
          << ") "
          << block->dump();

      return std::string{};
    }


  };

  class type_ast : public base_ast {
  public:
    virtual std::string dump() = 0;

  protected:
    explicit type_ast() { }
  };


// ...

  class variable_ast : public base_ast {
  public:
    explicit variable_ast(type_ast *type, std::string *name)
      : m_type(type), real_name(name) { }

    type_ast * get_type() {
      return m_type;
    }

    std::string &get_name() {
      return *real_name;
    }

    void set_name(std::string name) {
      *real_name = name;
    }
    virtual std::string dump() override {
      return "";
    }
  protected:
    type_ast *m_type;
    std::string *real_name;
  };

  class func_ast : public variable_ast {
  public:
    explicit func_ast(type_ast *type, std::string *name) : variable_ast(type, name) { }
    std::unique_ptr<base_ast> return_type;
    std::unique_ptr<base_ast> block;
    virtual std::string dump() override {
      std::cout << "define "
          << return_type->dump()
          << " @" << real_name << "("
      //  args
          << ") "
          << block->dump();

      return std::string{};
    }


  };

}


#endif
