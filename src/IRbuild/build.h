#ifndef BUILD_H
#define BUILD_H

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

#include "../../ast/compunit.h"

#include "llvm/IR/IRBuilder.h"

namespace pache {
inline namespace {
static std::unique_ptr<llvm::LLVMContext> TheContext =
    std::make_unique<llvm::LLVMContext>();
static std::unique_ptr<llvm::Module> TheModule =
    std::make_unique<llvm::Module>("first modlue", *TheContext);
// Create a new builder for the module.
static std::unique_ptr<llvm::IRBuilder<>> Builder =
    std::make_unique<llvm::IRBuilder<>>(*TheContext);
} // namespace
class base_build {
public:
  explicit base_build(base_build *father) : m_father(father) {}
  virtual ~base_build() = 0;

  virtual llvm::Value *find_var(std::string const &name) const = 0;

protected:
  base_build *m_father;
};

class build_block : public base_build {
public:
  explicit build_block(base_build *father) : base_build(father) {}
  virtual llvm::Value *find_var(std::string const &name) const override {
    if (m_symbol_table.contains(name)) {
      return m_symbol_table[name];
    } else if (m_father != nullptr) {
      return m_father->find_var(name);
    } else {
      std::cerr << "file_name: line: "
                << "name " << name << "not found!\n";
      return nullptr;
    }
  }
  void build(block_ast *ast);
  void insert(std::string const &name, llvm::Value *value) {
    if (find_var(name) == nullptr) {
      m_symbol_table[name] = value;
    } else {
      std::cerr << "file_name: line: "
                << "redifintion " << name << "\n";
    }
  }
  void deallco_all() {}

private:
  std::unordered_map<std::string, llvm::Value *> m_symbol_table;
};

class build_function : public base_build {
public:
  explicit build_function(base_build *father, func_ast *func)
      : base_build(father), m_block(father) {
    std::vector<llvm::Type *> args_Type;
    for (auto args : func->get_args_type()) {
      args_Type.push_back(args->codegen());
    }
    llvm::FunctionType *FT = llvm::FunctionType::get(
        func->get_return_type()->codegen(), args_Type, false);
    Function *F = Function::Create(FT, Function::ExternalLinkage,
                                   func->get_enconding_name(), TheModule.get());
  }
  void set_body(func_ast *func) {
    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
      Arg.setName(func->get_args_name()[Idx]);
    }

    llvm::BasicBlock *BB =
        llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
    Builder->SetInsertPoint(BB);
    for (auto &arg : F->args()) {
      m_block.insert(arg->getName(), &arg);
    }
    m_block.build(func->get_block());
    verifyFunction(*TheFunction);
  }

private:
  build_block m_block;
}

class builded_class {
public:
  explicit builded_class(
      /* std::string const &name,  */ llvm::StructType *llvm_type)
      : /* m_name(name),  */ m_type(llvm_type) {}

  void set_body(std::vector<std::unique_ptr<let_stmt>> &v_var) {
    std::vector<llvm::Type *> vars_type;
    for (auto &stmt_ptr : v_var) {
      vars_type.push_back(stmt_ptr->get_var_type());
    }
    m_type->setBody(vars_type);
    for (auto &stmt_ptr : v_var) {
      unsigned int i = 0;
      auto [it, b] = m_member_var.try_emplace(stmt_ptr->get_var_name(), i);
      if (!b) {
        std::cerr << "file name : line : error : redefinition of "
                  << tmt_ptr->get_var_name() << "\n";
      }
      i += 1;
    }
  }

  llvm::StructType *get_type() const { return m_type; }
  void create_member_functions(std::vector<std::unique_ptr<func_ast>> &v_func) {
    for (auto &func : v_func) {
      std::vector<llvm::Type *> args_Type;
      for (auto args : func->get_args_type()) {
        args_Type.push_back(agrs->codegen());
      }
      llvm::FunctionType *FT = llvm::FunctionType::get(
          func->get_return_type()->codegen(), args_Type, false);
      Function *F = Function::Create(FT, Function::ExternalLinkage,
                                     func->get_name(), TheModule.get());

      unsigned Idx = 0;
      for (auto &Arg : F->args()) {
        Arg.setName(func->get_args_name()[Idx]);
      }

      llvm::BasicBlock *BB =
          llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
      Builder->SetInsertPoint(BB);

      verifyFunction(*TheFunction);
    }
  }

  // return nullptr when not found name
  llvm::Value *get_member(llvm::Value *ptr, std::string const &name) {
    if (m_member_var.contains(name)) {
      return Builder->CreateGEP(
          m_type, ptr,
          {Builder->getInt32(0), Builder->getInt32(m_member_var[name])}, name);
    }
    throw std::logic_error{};
    // TODD log error

    // expmale
    // std::cerr << "file name: line: not found name " << name << "in class "
    //           << m_name << "\n";
  }

private:
  std::pmr::unordered_map<std::string, unsigned int> m_member_var;
  // std::string const m_name; // used by log error
  llvm::StructType *m_type;
  std::pmr::unordered_map<std::string, builded_class> builded_classes;
}

llvm::Constant *
build_global_variable(let_stmt *const stmt) {
  TheModule->insertGlobalVariable(new llvm::GlobalVariable{
      stmt->get_var_type()->codegen(), stmt->get_var_type()->isConstant(),
      llvm::GlobalValue::LinkageTypes::ExternalLinkage,
      stmt->get_init_exp()->codegen(), stmt->get_var_name()});
}

class file_build : public base_build {
public:
  file_build(compunit_ast *comp) : base_build(nullptr), ast(comp) {}

  llvm::StructType *class_name_find(std::string const &name) {
    auto it = class_def.find(name);
    if (it != class_def.end()) {
      return it->second;
    } else if (m_father != nullptr) {
      return m_father->class_name_find(name);
    } else {
      return nullptr;
    }
  }

private:
  void build_classes(compunit_ast *comp) {
    for (auto str : comp->class_def) {
      auto [it, b] = class_def.try_emplace(
          str->get_name(),
          builded_class{
              str->get_name(),
              llvm::StructType::create(*TheContext, str->encoding_name())});
      if (!b) {
        std::cerr << "file name : line : error : redefinition of "
                  << str->get_name() << "\n";
      }
    }
    for (auto str : comp->class_def) {
      auto builded = class_name_find(str->get_name());
      static_assert(builded != nullptr, "struct should be created.");
      builded.set_body(str->var_def);
    }
    for (auto var : comp->class_def->var_def) {
    }
  }
  void build_functions(std::vector<func_ast *> const &func_asts) {
    std::vector<std::pair<std::string, build_function>> functions;
    for (auto ast : func_asts) {
      functions.emplace(std::make_pair(ast->get_name(), build_function{ast}));
    }
    auto beg = functions.begin();
    for (auto ast : func_asts) {
      beg->set_body(ast);
      ++beg;
    }
    builded_functions.insert(functions.begin(), functions.end());
  }

  compunit_ast *ast; // maybe not need TODO
  std::unordered_map<std::string, builded_class> builded_classes;
  std::unordered_map<std::string, build_function> builded_functions;
};

build_call_exp(base_build *father, func_call_exp *const ast) {
  auto val = father->find_var(ast->get_name());
  if (val != nullptr) {
    // val should be a function type variable;

    // then get function
    return;
  }
  std::string name{}; // encoding name
  Function *CalleeF = TheModule->getFunction(name);
  if (CalleeF != nullptr) {
    std::vector<Value *> ArgsV;
    for (auto exp : ast->get_args()) {
      if (exp->codegen() != nullptr) {
        ArgsV.push_back(exp->codegen());
      } else {
        // log error
      }
    }

    return Builder->CreateCall(CalleeF, ArgsV, "calltmp");
  }
}

} // namespace pache