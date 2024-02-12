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
#include <memory>
#include <string_view>
#include <utility>
#include <vector>

namespace pache {
class_build::class_build(base_build *const father,
                         class_ast const &ast)
    : base_build(father) {
  for (auto &inner_class : ast.get_inner_class_def()) {
    auto [it, b] = builded_classes.try_emplace(
        inner_class->get_name(),
        std::make_unique<class_build>(this, *inner_class));
    if (!b) {
      // b is false meaing insert fail.
      std::cerr << "redefinition class\n";
    }
  }

  //m_type = class_type{llvm::StructType::create(vars_type, ast.get_name())}; 
  m_type = class_type{*this, ast};
  for (auto &func : ast.get_func_def()) {
   // TODO m_functions.emplace(std::make_pair(  func->get_name(), function_build{this, *func}));
  }
}

reference_ptr<build_variable >
class_build::find_var(std::string_view name) const {
  assert(false);
  return nullptr;
}
// class_build::class_build(
//     base_build *const father,
//     /* std::string_view  &name,  */ llvm::StructType *llvm_type)
//     : base_build(father), /* m_name(name),  */ m_type(llvm_type) {}
} // namespace pache
