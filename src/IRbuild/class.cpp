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

// class_build::class_build(
//     base_build *const father,
//     /* std::string_view  &name,  */ llvm::StructType *llvm_type)
//     : base_build(father), /* m_name(name),  */ m_type(llvm_type) {}
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
} // namespace pache
