#ifndef AST_H
#define AST_H

//#include "type.h"
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
#include <cstddef>
#include <iostream>
#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pache {

class variable_ast;
class stmt_ast;

class base_ast {
public:
  virtual ~base_ast() = default;
  // virtual llvm::Value *codegen() = 0;
  //  void print() { TheModule->print(llvm::errs(), nullptr); }
  void set_father(base_ast *father);
  base_ast *get_father();
  // virtual llvm::AllocaInst *find_named_Value(std::string const &name) const;
  //  virtual void insert_named_Value(std::string const &name,
  //                                  llvm::AllocaInst *val);
  /*  virtual std::string const decorated_name() const {
     using namespace std::literals;
     return ""s;
   } */

  // virtual variable_ast *find_dec(const std::string &name) const {
  //    return nullptr;
  //  }
  // virtual std::string location() = 0;

protected:
  base_ast *m_father;
  std::size_t m_line_number;
  // void insert_class_def(class_ast *p) {}
};
std::string get_father_decorated_name(base_ast *ast);

class callable_ast : public base_ast {};
class type_ast;
class variable_ast : public callable_ast {
public:
  explicit variable_ast(type_ast *type, std::string name)
      : m_type(type), real_name(name) {}

  type_ast *get_type() { return m_type; }
  // virtual llvm::Value *codegen() { return nullptr; } // TODO
  std::string const &get_name() const { return real_name; }

  void set_name(std::string name) { real_name = name; }

protected:
  type_ast *m_type;
  std::string real_name;
};

} // namespace pache

#endif
