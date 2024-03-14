#ifndef COMP_UNIT_H
#define COMP_UNIT_H

#include "../ast/compunit.h"
#include "build.h"
#include "class.h"
#include "expression.h"
#include "function.h"
#include "function_type.h"
#include "type.h"
#include "variable.h"
#include <functional>
#include <llvm-17/llvm/IR/Constant.h>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <unordered_set>
namespace pache {
class file_build : public base_build {
public:
  explicit file_build(compunit_ast const & comp);
  void make_statement_file();
  virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const override {
    // // m_father should be null
    // std::set<reference_ptr<function_build>> s;
    // auto [begin, end]{builded_functions.equal_range(name)};
    // while (begin != end) {
    //  // s.emplace(begin->second);
    //   ++begin;
    // }
    // return s;
  }

  virtual reference_ptr<build_variable>
  qualified_var_lookup(std::string_view name) override;
  virtual reference_ptr<build_type const>
  qualified_type_lookup(std::string_view name) const override;
  virtual reference_ptr<base_build>
  qualified_scope_lookup(std::string_view name) override;

private:
  std::unordered_map<std::string_view, class_build> builded_classes;
  std::unordered_map<std::string_view, std::unordered_map<reference_ptr<function_type const>, function_build>> builded_functions1;
  std::unordered_map<std::string_view, reference_ptr<file_build>>
      import_packges;
  std::unordered_map<std::string_view, std::unique_ptr<build_variable>>
      global_variables;
 // std::unordered_set<std::unique_ptr<scope_ast>> using_scope;

 void forward_statement(class_ast const & class_ast);
void forward_statement(func_ast const &func_ast);
reference_ptr<class_build> qualified_class_lookup(std::string_view name);

void define_variable(let_stmt const &ast);
auto forward_statement_function(func_ast const &ast)
 ->reference_ptr<function_build>;
};

std::unique_ptr<file_build> build_file(compunit_ast const & ast);
} // namespace pache

#endif
