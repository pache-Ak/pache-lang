#include "comp_unit.h"
#include "IRbuild/class.h"
#include "IRbuild/variable.h"
#include "function.h"

namespace pache {
file_build::file_build(compunit_ast *comp) : base_build(nullptr) {
  // packages

  for (auto &class_ast : comp->get_class()) {
    auto [it, b] = builded_classes.try_emplace(class_ast->get_name(),
                                               class_build(this, class_ast));
    // b is false meaning insert error
    if (!b) {
      std::cerr << "redefine class.\n";
    }
  }

  for (auto &func_ast : comp->get_funcs_ast()) {
    auto [it, b] = builded_functions.try_emplace(
        func_ast->get_name(), function_build(this, func_ast));
    // b is false meaning insert error
    if (!b) {
      std::cerr << "redefine function.\n";
    }
  }

  for (auto &var : comp->get_var()) {
    auto type = type_build(this, var->get_var_type());
    TheModule->getOrInsertGlobal(var->get_var_name(), type->get_llvm_type());
    llvm::GlobalVariable *gVar = TheModule->getNamedGlobal(var->get_var_name());
    auto [it, b] = global_variables.try_emplace(
        var->get_var_name(), build_global_variable(std::move(type), gVar));
    // b is false meaning insert error
    if (!b) {
      std::cerr << "redefine variable.\n";
    }
  }
}
std::unique_ptr<build_variable> const &
file_build::find_var(std::string const &name) const {
  auto it = global_variables.find(name);
  if (it != global_variables.end()) {
    return it->second;
  } else {
    for (auto file : import_packges) {
      if (file.second.get().find_var(name) != nullptr) {
        return file.second.get().find_var(name);
      }
    }
    return nullptr;
  }
}

} // namespace pache