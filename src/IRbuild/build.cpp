#include "build.h"
#include "type.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <iostream>
#include <memory>

namespace pache {
inline namespace IR {


std::unique_ptr<llvm::LLVMContext> TheContext;
    
std::unique_ptr<llvm::Module> TheModule;
// Create a new builder for the module.
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
void InitializeModule()  {
  // Open a new context and module.
  TheContext = std::unique_ptr<llvm::LLVMContext>{ new llvm::LLVMContext()};
  TheModule =std::unique_ptr<llvm::Module> {new llvm::Module("first modlue", *TheContext)};
  // Create a new builder for the module.
  Builder  = std::unique_ptr<llvm::IRBuilder<>>{new llvm::IRBuilder<>(*TheContext)} ;
  std::cout << pache::TheContext.get() <<"\n";
  std::cout << pache::TheModule.get() <<"\n";
  std::cout << pache::Builder.get() <<"\n";
  
  std::cout << "ini finsh.\n";
}
}
base_build::~base_build(){}
std::unordered_map<std::string_view, std::unique_ptr<build_type>> const
base_build::prprimary_type{
  []{
    using namespace std::literals::string_view_literals;

    std::unordered_map<std::string_view, std::unique_ptr<build_type>> val;

    val.emplace("void"sv, std::make_unique<void_type_t>());
    val.emplace("bool"sv, std::make_unique<bool_type_t>());
    val.emplace("s"sv,    std::make_unique<size_type_t>());
    
    return val;
  }()
};

// std::function<std::unique_ptr<build_variable>(std::intmax_t)>
// base_build::find_integer_literal_operator(std::string_view suffix) {
//   auto search = integer_literal_operators.find(suffix);
//   if (search != integer_literal_operators.end()) {
//     return search->second;
//   } else if (m_father != nullptr) {
//     return m_father->find_integer_literal_operator(suffix);
//   } else {
//     return {};
//   }
// }

reference_ptr<base_build> base_build::find_scope(std::string_view name) {
  if (auto p = qualified_scope_lookup(name); p != nullptr) {
    return p;
  } else if (m_father != nullptr) {
    return m_father->find_scope(name);
  }
}
reference_ptr<build_variable> base_build::find_var(std::string_view name) {
  if (auto p = qualified_var_lookup(name); p != nullptr) {
    return p;
  } else if (m_father != nullptr) {
    return m_father->find_var(name);
  } else {
    return nullptr;
  }
}
reference_ptr<build_type const>
base_build::find_type(std::string_view name) const {
  if (auto p = prprimary_type.find(name); p != nullptr) {
    return p->second;
  } else if (auto p = qualified_type_lookup(name); p != nullptr) {
    return p;
  } else if (m_father != nullptr) {
    return m_father->find_type(name);
  } else {
    return nullptr;
  }
}
} // namespace pache