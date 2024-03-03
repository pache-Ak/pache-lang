#ifndef IR_BUILD_H
#define IR_BUILD_H

#include "llvm/ADT/APFloat.h"
#include "llvm/ADT/STLExtras.h"
#include "llvm/IR/BasicBlock.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/PassManager.h"
#include "llvm/IR/Type.h"
#include "llvm/IR/Verifier.h"
#include "llvm/Passes/PassBuilder.h"
#include "llvm/Passes/StandardInstrumentations.h"
#include "llvm/Support/TargetSelect.h"
#include "llvm/Target/TargetMachine.h"
#include "llvm/Transforms/InstCombine/InstCombine.h"
#include "llvm/Transforms/Scalar.h"
#include "llvm/Transforms/Scalar/GVN.h"
#include "llvm/Transforms/Scalar/Reassociate.h"
#include "llvm/Transforms/Scalar/SimplifyCFG.h"
#include "llvm/Transforms/Utils.h"
//#include <cstdint>
//#include <functional>
#include <memory>
#include <set>
#include <string>
#include <string_view>
#include <unordered_map>
#include "../reference_ptr.h"
#include "IRbuild/scope_ref.h"

namespace pache {

inline namespace IR {
extern std::unique_ptr<llvm::LLVMContext> TheContext;
extern std::unique_ptr<llvm::Module> TheModule;
extern std::unique_ptr<llvm::IRBuilder<>> Builder;
extern std::unique_ptr<llvm::FunctionPassManager> TheFPM;
extern std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
extern std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
extern std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
extern std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
extern std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
extern std::unique_ptr<llvm::StandardInstrumentations> TheSI;
void InitializeModuleAndManagers();
} // namespace IR


class build_type;
class build_variable;
class function_build;
class base_build {
public:
  base_build &operator=(const base_build &) = delete;
  base_build &operator=(base_build &&) = delete;

  explicit base_build(base_build *const father, std::string &&decorated_name) : m_father(father), m_decorated_name(std::move(decorated_name)) {}
  virtual ~base_build() = 0;

  reference_ptr<build_variable> find_var(std::string_view name);
  reference_ptr<build_type const> find_type(std::string_view name) const;
  reference_ptr<base_build> find_scope(std::string_view name);

  virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const = 0;
  virtual reference_ptr<build_variable>
  qualified_var_lookup(std::string_view name) = 0;
  virtual reference_ptr<build_type const> qualified_type_lookup(std::string_view name) const = 0;
  virtual reference_ptr<base_build> qualified_scope_lookup(std::string_view name)  = 0;
  base_build *const m_father;

  std::string decorated_name() const {
    return m_decorated_name;
  }

  virtual bool is_block() const {
    return false;
  }
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
  std::string m_decorated_name;
  };


} // namespace pache

#endif
