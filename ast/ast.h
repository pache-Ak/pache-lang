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
#include <sstream>

namespace pache {
  inline namespace {
    std::size_t ssa_value = 0;
  }


  class base_ast {
  public:
    virtual ~base_ast() = default;
    virtual std::string dump() const = 0;
  };

  class compunit_ast : public base_ast {
  public:
    std::unique_ptr<base_ast> func_def;
    virtual std::string dump() const override {
      std::cout << "target datalayout = \"e-m:e-p270:32:32-p271:32:32-p272:64:64-i64:64-f80:128-n8:16:32:64-S128\"\n";
      std::cout << "target triple = \"x86_64-pc-linux-gnu\"\n\n";

      return func_def->dump();
    }


  };

  class func_ast : public base_ast {
  public:
    std::string ident;
    std::unique_ptr<base_ast> return_type;
    std::unique_ptr<base_ast> block;
    virtual std::string dump() const override {
      std::ostringstream out;
      std::cout << "define "
          << return_type->dump()
          << " @" << ident << "("
      //  args
          << ") "
          << block->dump();

      return std::string{};
    }


  };

  class type_ast : public base_ast {
  public:
    type_ast(std::string s) : ident(s) { }
    std::string ident;
    virtual std::string dump() const override {
      return ident;
    }


  };


// ...




}


#endif
