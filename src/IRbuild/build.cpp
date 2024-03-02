#include "build.h"
#include "type.h"
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
#include <iostream>
#include <memory>

namespace pache {
inline namespace IR {
std::unique_ptr<llvm::LLVMContext> TheContext;
std::unique_ptr<llvm::Module> TheModule;
std::unique_ptr<llvm::IRBuilder<>> Builder;
std::unique_ptr<llvm::FunctionPassManager> TheFPM;
std::unique_ptr<llvm::LoopAnalysisManager> TheLAM;
std::unique_ptr<llvm::FunctionAnalysisManager> TheFAM;
std::unique_ptr<llvm::CGSCCAnalysisManager> TheCGAM;
std::unique_ptr<llvm::ModuleAnalysisManager> TheMAM;
std::unique_ptr<llvm::PassInstrumentationCallbacks> ThePIC;
std::unique_ptr<llvm::StandardInstrumentations> TheSI;

void InitializeModuleAndManagers()  {
  // Open a new context and module.
  TheContext = std::make_unique<llvm::LLVMContext>();
  TheModule = std::make_unique<llvm::Module>("KaleidoscopeJIT", *TheContext);

  // Create a new builder for the module.
  Builder  = std::make_unique<llvm::IRBuilder<>>(*TheContext);

  // Create new pass and analysis managers.
  TheFPM = std::make_unique<llvm::FunctionPassManager>();
  TheLAM = std::make_unique<llvm::LoopAnalysisManager>();
  TheFAM = std::make_unique<llvm::FunctionAnalysisManager>();
  TheCGAM = std::make_unique<llvm::CGSCCAnalysisManager>();
  TheMAM = std::make_unique<llvm::ModuleAnalysisManager>();
  ThePIC = std::make_unique<llvm::PassInstrumentationCallbacks>();
  TheSI = std::make_unique<llvm::StandardInstrumentations>(*TheContext,
                                                     /*DebugLogging*/ true);
  TheSI->registerCallbacks(*ThePIC, TheMAM.get());

  // Add transform passes.
  // Do simple "peephole" optimizations and bit-twiddling optzns.
  TheFPM->addPass(llvm::InstCombinePass());
  // Reassociate expressions.
  TheFPM->addPass(llvm::ReassociatePass());
  // Eliminate Common SubExpressions.
  TheFPM->addPass(llvm::GVNPass());
  // Simplify the control flow graph (deleting unreachable blocks, etc).
  TheFPM->addPass(llvm::SimplifyCFGPass());

  // Register analysis passes used in these transform passes.
  llvm::PassBuilder PB;
  PB.registerModuleAnalyses(*TheMAM);
  PB.registerFunctionAnalyses(*TheFAM);
  PB.crossRegisterProxies(*TheLAM, *TheFAM, *TheCGAM, *TheMAM);
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
    val.emplace("size"sv, std::make_unique<size_type_t>());
    val.emplace("byte"sv, std::make_unique<byte_t>());
    val.emplace("i8"sv,   std::make_unique<i8_type_t>());
    val.emplace("i16"sv,  std::make_unique<i16_type_t>());
    val.emplace("i32"sv,  std::make_unique<i32_type_t>());
    val.emplace("i64"sv,  std::make_unique<i64_type_t>());
  //  val.emplace("i128"sv, std::make_unique<i128_type_t>());
    val.emplace("u8"sv,   std::make_unique<u8_type_t>());
    val.emplace("u16"sv,  std::make_unique<u16_type_t>());
    val.emplace("u32"sv,  std::make_unique<u32_type_t>());
    val.emplace("u64"sv,  std::make_unique<u64_type_t>());
  //  val.emplace("u128"sv, std::make_unique<u128_type_t>());
    val.emplace("f16"sv,  std::make_unique<f16_type_t>());
    val.emplace("f32"sv,  std::make_unique<f32_type_t>());
    val.emplace("f64"sv,  std::make_unique<f64_type_t>());
    val.emplace("f128"sv, std::make_unique<f128_type_t>());
  //  val.emplace("d32"sv,  std::make_unique<d32_type_t>());
  //  val.emplace("d64"sv,  std::make_unique<d64_type_t>());
  //  val.emplace("d128"sv, std::make_unique<d128_type_t>());
    val.emplace("c8"sv,   std::make_unique<c8_type_t>());
    val.emplace("c16"sv,  std::make_unique<c16_type_t>());
    val.emplace("c32"sv,  std::make_unique<c32_type_t>());
    
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