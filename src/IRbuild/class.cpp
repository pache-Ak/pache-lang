#include "class.h"
#include "../ast/class.h"
#include "build.h"
#include "function.h"
#include "variable.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Verifier.h"
#include <cassert>
#include <memory>

namespace pache {
class_build::class_build(base_build *const father,
                         std::unique_ptr<class_ast> const &ast)
    : base_build(father) {
  for (auto &inner_class : ast->get_inner_class_def()) {
    auto [it, b] = builded_classes.try_emplace(
        inner_class->get_name(),
        std::make_unique<class_build>(this, inner_class));
    if (!b) {
      // b is false meaing insert fail.
      std::cerr << "redefinition class\n";
    }
  }

  std::vector<llvm::Type *> vars_type;
  size_t i = 0;
  for (auto &var : ast->get_var_def()) {
    auto [it, b] = m_member_var.try_emplace(
        ast->get_name(), type_build(father, var->get_var_type()), ++i);
    if (b) {
      // b is true meaing insert succes.
      vars_type.emplace_back(it->second.get_type()->get_llvm_type());
    } else {
      // b is false meaing insert fail.
      std::cerr << "duplicate member.\n";
    }
  }

  m_type = llvm::StructType::create(vars_type, ast->get_name());

  for (auto &func : ast->get_func_def()) {
    m_functions.emplace(func->get_name(), function_build(this, func.get()));
  }
}

std::unique_ptr<build_variable> const &
class_build::find_var(std::string const &name) const {
  assert(false);
  return nullptr;
}

llvm::Value *class_build::get_member_var(llvm::Value *ptr,
                                         std::string const &name) {
  auto it = m_member_var.find(name);
  if (it != m_member_var.end()) {
    return IR::Builder->CreateGEP(
        m_type, ptr,
        {IR::Builder->getInt32(0), IR::Builder->getInt32(it->second.get_num())},
        name);
  } else {
    return find_var(name)->get_value();
  }
}
} // namespace pache
