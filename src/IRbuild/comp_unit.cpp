#include "comp_unit.h"
#include "IRbuild/class.h"
#include "IRbuild/variable.h"
#include "build.h"
#include "function.h"
#include <fstream>
#include <iostream>
#include <istream>
#include <memory>
#include <fstream>
#include <unordered_set>
#include "../reference_ptr.h"

namespace pache {
file_build::file_build(compunit_ast const *const comp) : base_build(nullptr) {
  // packages
  std::ifstream pcm_in;
  for (auto &package : comp->get_packages()) {
    // find file by name
    
    for (auto const &dir : comp->get_include_dir()) {
      pcm_in.open(dir / package->get_file_name());
      if (pcm_in.is_open()) {
        


        break;
      } else {
        continue;
      }
    }

  }

  for (auto &class_ast : comp->get_class()) {
    auto [it, b] = builded_classes.try_emplace(class_ast->get_name(),
                                               class_build(this, *class_ast));
    // b is false meaning insert error
    if (!b) {
      std::cerr << "redefine class.\n";
    }
  }

  for (auto &func_ast : comp->get_funcs_ast()) {

    auto it= builded_functions.emplace(
        func_ast->get_name(), std::make_unique<function_build>(this, *func_ast));
    // b is false meaning insert error
    if (0) { // TODO
      std::cerr << "redefine function.\n";
    }
  }

  for (auto &var : comp->get_var()) {
    auto type = type_build(*this, var->get_var_type());
    TheModule->getOrInsertGlobal(var->get_var_name(), type->get_llvm_type());
    llvm::GlobalVariable *gVar = TheModule->getNamedGlobal(var->get_var_name());
    auto [it, b] = global_variables.try_emplace(
        var->get_var_name(),std::make_unique<build_global_variable>( std::move(type), gVar));
    // b is false meaning insert error
    if (!b) {
      std::cerr << "redefine variable.\n";
    }
  }
}
std::unique_ptr<build_type> file_build::find_type(std::string_view name) const {
  if (auto it = builded_classes.find(name); it != builded_classes.end()) {
    return it->second.get_type().clone();
  } else if (m_father != nullptr) {
    return m_father->find_type(name);
  } else {
    return nullptr;
  }
}
void file_build::make_statement_file() {}
reference_ptr<build_variable >
file_build::find_var(std::string_view name) const {
  std::unordered_set<reference_ptr<build_variable>> s;
  if (auto it = global_variables.find(name); it != global_variables.end()) {
    s.emplace(it->second);
  } 
  for (auto file : import_packges) {
    if (file.second.get().find_var(name) != nullptr) {
      s.emplace(file.second.get().find_var(name));
    }
  }
  // using import space

  if (s.empty()) {
    // TODO log error
    // not find
  } else if (s.size() == 1) {
    return *s.begin();
  } else {
    // TODO  log error
    // is amibious
  }
    return nullptr;
}

std::unique_ptr<file_build> build_file(compunit_ast const *const ast) {
  return std::make_unique<file_build>(ast);
}
} // namespace pache