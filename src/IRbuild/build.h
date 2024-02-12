#ifndef IR_BUILD_H
#define IR_BUILD_H

#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/LegacyPassManager.h"
//#include <cstdint>
//#include <functional>
#include <memory>
#include <set>
#include <string_view>
#include <unordered_map>
#include "../reference_ptr.h"
#include "IRbuild/scope_ref.h"

namespace pache {

inline namespace IR {
static std::unique_ptr<llvm::LLVMContext> TheContext;
    
static std::unique_ptr<llvm::Module> TheModule;
// Create a new builder for the module.
static std::unique_ptr<llvm::IRBuilder<>> Builder;
static std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM = std::make_unique<llvm::legacy::FunctionPassManager>(TheModule.get());
void InitializeModule();
} // namespace IR


class build_type;
class build_variable;
class function_build;
class base_build {
public:
  base_build &operator=(const base_build &) = delete;
  base_build &operator=(base_build &&) = delete;

  explicit base_build(base_build *const father) : m_father(father) {}
  virtual ~base_build() = 0;

  virtual reference_ptr<build_variable>
  find_var(std::string_view name) const = 0;
  virtual std::unique_ptr<build_type> find_type(std::string_view name) const = 0;

  
  virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const = 0;
protected:
  base_build(base_build &&) = default;
  base_build(base_build const &other) = default;
 // implicitlt delete
 // base_build &operator=(base_build const &other) = default;
  

  // point to father scope maybe is nullptr
  base_build *const m_father;

  static std::unordered_map<
    std::string_view, std::unique_ptr<build_type>
    > const prprimary_type;
  std::unordered_map<std::string_view, scope_ref> m_using_scope;
  };


} // namespace pache

#endif
