#include "class.h"
#include "../ast/class.h"
#include "build.h"
#include "class_type.h"
#include "function.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Verifier.h"
#include <iostream>
#include <llvm-17/llvm/IR/DerivedTypes.h>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>
#include <sstream>

namespace pache {
std::string name_mangling(base_build const &scope, std::string_view class_name) {
  using namespace std::literals::string_literals;

  std::ostringstream oss;
  oss << std::hex << class_name.size();

  return scope.decorated_name() + "_S" + oss.str()  + std::string(class_name);
}




reference_ptr<build_variable>
class_build::qualified_var_lookup(std::string_view name)  {
  return nullptr;
  // TODO add static vaiables;
}
reference_ptr<build_type const>
class_build::qualified_type_lookup(std::string_view name) const {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return &it->second->get_type();
  } else {
    return nullptr;
  }
}
reference_ptr<base_build>
class_build::qualified_scope_lookup(std::string_view name)  {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return reference_ptr<base_build>(it->second);
  } else {
    return nullptr;
  }
}

class_build forward_statement(base_build &build, class_ast const &ast) {
  auto name = name_mangling(build, ast.get_name());
  auto type = llvm::StructType::create(*TheContext, name);
  return class_build(
    &build, std::move(name),
    type
  );
}

void define(class_build &cla, class_ast const &ast) {

}
void class_build::forward_statement_class(class_ast const &inner) {
  auto [it, b] = builded_classes.try_emplace(
      inner.get_name(), std::make_unique<class_build>(pache::forward_statement(*this, inner)));

  // b is false meaning insert error
  if (!b) {
    std::cerr << "redefine class.\n";
  }
}
void class_build::forward_statement_inner_classes(class_ast const &ast) {
  for (auto &class_ast : ast.get_inner_class_def()) {
    forward_statement_class(*class_ast);
  }
}
void class_build::define_class_body(class_ast const &ast) {
  for (auto const &inner_ast : ast.get_inner_class_def()) {
    if (auto inner = qualified_class_lookup(inner_ast->get_name());
        inner != nullptr) {
      inner->define_class_body(*inner_ast);
    }
  }

  m_type.define_body(*this, ast);
}
reference_ptr<class_build>
class_build::qualified_class_lookup(std::string_view name) {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return it->second;
  } else {
    return nullptr;
  }
}
auto class_build::forward_statement_class_function(func_ast const &ast)
    -> reference_ptr<function_build> {
  auto func = pache::forward_statement(*this, ast);

  if (!func.has_value()) {
    std::cerr << "statement function error.\n";
    return nullptr;
  }

  auto [it, b] = builded_functions1[ast.get_name()].try_emplace(
      &func.value().get_type(), std::move(func.value()));
  if (!b) {
    std::cerr << "redefine function.\n";
    return nullptr;
  }
  return &it->second;
}
} // namespace pache
