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
extern std::unique_ptr<llvm::LLVMContext> TheContext;
    
extern std::unique_ptr<llvm::Module> TheModule;
// Create a new builder for the module.
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::legacy::FunctionPassManager> TheFPM;
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
  virtual reference_ptr<build_type const> find_type(std::string_view name) const = 0;

  
  virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const = 0;
  virtual reference_ptr<build_variable>
  qualified_var_lookup(std::string_view name) = 0;
  virtual reference_ptr<build_type const> qualified_type_lookup(std::string_view name) const = 0;
  virtual reference_ptr<base_build> qualified_scope_lookup(std::string_view name)  = 0;
  base_build *const m_father;
protected:
  base_build(base_build &&) = default;
  base_build(base_build const &other) = default;
 // implicitlt delete
 // base_build &operator=(base_build const &other) = default;
  

  // point to father scope maybe is nullptr

  static std::unordered_map<
    std::string_view, std::unique_ptr<build_type>
    > const prprimary_type;
  std::unordered_map<std::string_view, scope_ref> m_using_scope;
  };


} // namespace pache

#endif
