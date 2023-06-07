#ifndef AST_H
#define AST_H

#include "type.h"
// #include "llvm/ADT/APFloat.h"
// #include "llvm/ADT/STLExtras.h"
// #include "llvm/IR/BasicBlock.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/Instructions.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/LegacyPassManager.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/Type.h"
// #include "llvm/IR/Verifier.h"
// #include "llvm/Support/TargetSelect.h"
// #include "llvm/Target/TargetMachine.h"
// #include "llvm/Transforms/InstCombine/InstCombine.h"
// #include "llvm/Transforms/Scalar.h"
// #include "llvm/Transforms/Scalar/GVN.h"
#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pache {

inline namespace {
static std::unique_ptr<llvm::LLVMContext> TheContext;
static std::unique_ptr<llvm::Module> TheModule;
// Create a new builder for the module.
static std::unique_ptr<llvm::IRBuilder<>> Builder;

} // namespace

class variable_ast;
class stmt_ast;
class base_ast {
public:
  virtual ~base_ast() = default;
  virtual llvm::Value *codegen() = 0;
  // void print() { TheModule->print(llvm::errs(), nullptr); }
  void set_father(base_ast *father);
  base_ast *get_father();
  // virtual llvm::AllocaInst *find_named_Value(std::string const &name) const;
  //  virtual void insert_named_Value(std::string const &name,
  //                                  llvm::AllocaInst *val);
  virtual std::string const encoding_name() const {
    using namespace std::literals;
    return ""s;
  }

  // virtual variable_ast *find_dec(const std::string &name) const {
  //    return nullptr;
  //  }

protected:
  base_ast *m_father;

  // void insert_class_def(class_ast *p) {}
};

class callable_ast : public base_ast {};

class variable_ast : public callable_ast {
public:
  explicit variable_ast(type_ast *type, std::string name)
      : m_type(type), real_name(name) {}

  type_ast *get_type() { return m_type; }
  virtual llvm::Value *codegen() { return nullptr; } // TODO
  std::string const &get_name() const { return real_name; }

  void set_name(std::string name) { real_name = name; }

protected:
  type_ast *m_type;
  std::string real_name;
};

// ...
class func_arg : public variable_ast {
public:
  explicit func_arg(type_ast *type, std::string &&str)
      : variable_ast(type, std::move(str)) {}

private:
};

class func_ast : public callable_ast {
public:
  explicit func_ast(std::string &&name,
                    std::pair<std::vector<pache::type_ast *>,
                              std::vector<std::string>> &&args,
                    type_ast *return_type, base_ast *block)
      : m_name(std::move(name)), m_type(return_type),
        m_args_type(std::move(args.first)), m_args_name(std::move(args.second)),
        m_block(block) {}

  virtual llvm::Value *codegen() override;
  /*virtual variable_ast *find_dec(const std::string &name) const override {
    auto beg = std::find_if(m_args.begin(), m_args.end(), [=](func_arg *arg) {
      return (arg->get_name() == name);
    });
    if (beg != m_args.end()) {
      return *beg;
    } else {
      return m_father->find_dec(name);
    }
  }*/
  std::vector<type_ast *> const &get_args_type() const { return m_args_type; }
  std::vector<std::string> const &get_args_name() const { return m_args_name; }
  type_ast *const get_return_type() const { return m_type; }
  std::string const &get_name() const { return m_name; }
  std::string const get_encoding_name() const {
    std::string s = "_N" + m_father->encoding_name() + get_name();
    for (auto &type : m_args_type) {
      s += type->encoding_name();
    }
    return s;
  }

protected:
  std::string m_name;
  type_ast *m_type;
  std::vector<type_ast *> m_args_type;
  std::vector<std::string> m_args_name;
  std::unique_ptr<base_ast> m_block;
  std::string name_mangling() {
    std::string s{};
    s += get_father()->encoding_name();
    s += this->m_name;
    for (auto arg : m_args_type) {
      s += arg->encoding_name();
    }
    return s;
  }
};
/* class main_func_ast : public func_ast {
public:
  explicit main_func_ast(std::string &&name, std::vector<func_arg *> args,
                         type_ast *return_type, base_ast *block)
      : func_ast(std::move(name), std::move(args), return_type, block) {}
}; */

} // namespace pache

#endif
