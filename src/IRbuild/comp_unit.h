#ifndef COMP_UNIT_H
#define COMP_UNIT_H

#include "ast/compunit.h"
#include "build.h"
#include "class.h"
#include "type.h"
namespace pache {
class file_build : public base_build {
public:
  file_build(compunit_ast *comp) : base_build(nullptr), ast(comp) {}

  virtual llvm::StructType *find_type(std::string const &name) override {
    auto it = class_def.find(name);
    if (it != class_def.end()) {
      return it->second;
    } else if (m_father != nullptr) {
      return m_father->find_type(name);
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
  std::unordered_map<std::string, class_build> builded_classes;
  std::unordered_map<std::string, build_function> builded_functions;
};

llvm::Constant *build_global_variable(let_stmt *const stmt) {
  TheModule->insertGlobalVariable(new llvm::GlobalVariable{
      stmt->get_var_type()->codegen(), stmt->get_var_type()->isConstant(),
      llvm::GlobalValue::LinkageTypes::ExternalLinkage,
      stmt->get_init_exp()->codegen(), stmt->get_var_name()});
}
} // namespace pache