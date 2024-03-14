#include "comp_unit.h"
#include "IRbuild/class.h"
#include "IRbuild/variable.h"
#include "build.h"
#include "class_type.h"
#include "expression.h"
#include "function.h"
#include <fstream>
#include <iostream>
#include <llvm-17/llvm/IR/GlobalVariable.h>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <utility>
#include <variant>
#include "../reference_ptr.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Constant.h"
#include "llvm/IR/Value.h"
#include "llvm/Support/Casting.h"
#include <string_view>
#include <vector>

namespace pache {
void file_build::forward_statement(class_ast const & class_ast) {
  auto [it, b] = builded_classes.try_emplace(
    class_ast.get_name(),
    pache::forward_statement(*this, class_ast)
  );
  
  // b is false meaning insert error
  if (!b) {
    std::cerr << "redefine class.\n";
  }

  it->second.forward_statement_inner_classes(class_ast);
}

void file_build::forward_statement(func_ast const &func_ast) {

}

file_build::file_build(compunit_ast const & comp) : base_build(nullptr, "") {
  // packages TODO

  // 1 pass statement class and inner class
  for (auto &class_ast : comp.get_class()) {
    forward_statement(*class_ast);
  }

  // 2pass statement functions
  std::unordered_map<
    reference_ptr<func_ast>,
    reference_ptr<function_build>
  > relation;
  for (auto &func_ast : comp.get_funcs_ast()) {
    auto it = forward_statement_function(*func_ast);
    if (it == nullptr) {
      continue;
    }

    relation.emplace(func_ast, it);
  }

  for (auto &class_ast : comp.get_class()) {
    for (auto &func_ast : class_ast->get_func_def()) {
      auto c = qualified_class_lookup(class_ast->get_name());
      auto it = c->forward_statement_class_function(*func_ast);
      if (it == nullptr) {
        continue;
      }

    }
  }

  // 2pass define class
  for (auto &class_ast : comp.get_class()) {
    auto c = qualified_class_lookup(class_ast->get_name());
    c->define_class_body(*class_ast);
  }

    //  3pass define variable
  for (auto &var : comp.get_var()) {
    define_variable(*var);
  }

  // 4 pass define function
  for (auto &func_ast : comp.get_funcs_ast()) {
    if (auto it = relation.find(func_ast); it != relation.end()) {
      it->second->define(this, *func_ast);
    }
  }

  for (auto &class_ast : comp.get_class()) {
    for (auto &func_ast : class_ast->get_func_def()) {

      if (auto it = relation.find(func_ast); it != relation.end()) {
        it->second->define(this, *func_ast);
      }

    }
  }  


}

void file_build::make_statement_file() {}

std::unique_ptr<file_build> build_file(compunit_ast const & ast) {
  return std::make_unique<file_build>(ast);
}
reference_ptr<build_variable>
file_build::qualified_var_lookup(std::string_view name)  {
  if (auto it = global_variables.find(name); it != global_variables.end()) {
    return it->second;
  } else {
    std::cout << name << "\terr find var in qualified  in root.\n";
    for (auto const &[f, s] : global_variables) {
      std::cout << f << "\n";
    }
    return nullptr;
  }
}
reference_ptr<build_type const>
file_build::qualified_type_lookup(std::string_view name) const {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return &it->second.get_type();
  } else {
    return nullptr;
  }
}
reference_ptr<base_build>
file_build::qualified_scope_lookup(std::string_view name) {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}
reference_ptr<class_build>
file_build::qualified_class_lookup(std::string_view name) {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return &it->second;
  } else {
    return nullptr;
  }
}
void file_build::define_variable(let_stmt const &ast) {
  auto type = type_build(*this, ast.get_var_type());
  if (type == nullptr) {
    std::cout << "type error\n";
    return;
  }

  if (global_variables.find(ast.get_var_name()) != global_variables.end()) {
    std::cerr << "redefine variable:\t" << ast.get_var_name() << "\n";
    return;
  }

  llvm::Constant *value;
  if (ast.get_init_exp() != nullptr) {
    auto var = build_literal(*this, *ast.get_init_exp());
    if (var == nullptr) {
      std::cerr << "build literal error.\n";
      return;
    }
    value = var->get_value();
  } else {
    value = nullptr;
  }

  llvm::GlobalVariable *globalVar = new llvm::GlobalVariable(
      *TheModule, type->get_llvm_type(), false, llvm::GlobalValue::ExternalLinkage, value,
      ast.get_var_name());

  global_variables.emplace(
        ast.get_var_name(),
        std::make_unique<build_global_variable>(std::move(type), globalVar)
  );
}
auto file_build::forward_statement_function(func_ast const &ast) 
-> reference_ptr<function_build>{
  auto func = pache::forward_statement(*this, ast);

  if (!func.has_value()) {
    std::cerr << "statement function error.\n";
    return nullptr;
  }

  auto [it, b] = builded_functions1[ast.get_name()].try_emplace(&func.value().get_type(), std::move(func.value()));
  if (!b) {
    std::cerr << "redefine function.\n";
    return nullptr;
  }
  return &it->second;
}


} // namespace pache