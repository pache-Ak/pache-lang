#include "expression.h"
#include "../IRbuild/expression.h"
#include "../IRbuild/function.h"
#include <algorithm>
#include <array>
#include <functional>
#include <memory>
#include <vector>
using namespace std::literals;

namespace pache {
std::unique_ptr<build_variable> build_exp(std::unique_ptr<exp_ast> const &ast) {
  return ast->build();
}

std::unique_ptr<build_variable> pache::unary_plus::build() const {
  std::array<std::unique_ptr<build_variable>, 1> arges;

  auto it = arges.begin();
  auto beg = m_arguments.begin();
  for (; beg != m_arguments.end(); ++it, ++beg) {
    *it = build_exp(*beg);
  }

  std::unique_ptr<function_build> const &func =
      function_lookup("operator+"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    args_Value[0] = arges[0]->get_value();

    return std::make_unique<build_prvalue_variable>(
        func->get_function_type()->get_return_type(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator+"));
  } else {
    return nullptr;
  }
}

std::unique_ptr<build_variable> pache::unary_minus::build() const {
  std::array<std::unique_ptr<build_variable>, 1> arges;

  auto it = arges.begin();
  auto beg = m_arguments.begin();
  for (; beg != m_arguments.end(); ++it, ++beg) {
    *it = build_exp(*beg);
  }

  std::unique_ptr<function_build> const &func =
      function_lookup("operator-"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    args_Value[0] = arges[0]->get_value();

    return std::make_unique<build_prvalue_variable>(
        func->get_function_type()->get_return_type(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator-"));
  } else {
    return nullptr;
  }
}
} // namespace pache

// llvm::Value *pache::func_call_exp::codegen() {
//   llvm::Function *CalleeF = TheModule->getFunction(m_name);
//   if (CalleeF == nullptr) {
//     // TODO log error
//   }

//   if (CalleeF->arg_size() != m_args.size()) {
//     // TODO log error
//   }

//   std::vector<llvm::Value *> args_Value;
//   for (auto val : m_args) {
//     args_Value.push_back(val->codegen());
//     if (args_Value.back() == nullptr) {
//       return nullptr;
//     }
//   }

//   return Builder->CreateCall(CalleeF, args_Value, "calltmp");
// }

// llvm::Value *pache::var_exp::codegen() {
//   /*   llvm::Value *val = get_father()->find_named_Value(m_name);
//     if (val == nullptr) {
//       // TODO log error
//     }

//     return val; */

//   return nullptr;
// }

// llvm::Value *pache::binary_mul_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateMul(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFMul(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return nullptr;
//     // TODO log error
//   }
// }

// llvm::Value *pache::binary_div_exp::codegen() {
//   /*   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//       if (m_lhs->get_type()->is_signed()) {
//         return Builder->CreateSDiv(m_lhs->codegen(), m_rhs->codegen(), "");
//       } else if (m_lhs->get_type()->is_unsigned()) {
//         return Builder->CreateUDiv(m_lhs->codegen(), m_rhs->codegen(), "");
//       } else {
//         return nullptr;
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFDiv(m_lhs->codegen(), m_rhs->codegen(), "");
//     } else {
//       return nullptr;
//       // TODO log error
//     } */
// }

// llvm::Value *pache::binary_mod_exp::codegen() {
//   /*  if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//      if (m_lhs->get_type()->is_signed()) {
//        return Builder->CreateSRem(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else if (m_lhs->get_type()->is_unsigned()) {
//        return Builder->CreateURem(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else {
//        return nullptr;
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFRem(m_lhs->codegen(), m_rhs->codegen(), "");
//    } else {
//      return nullptr;
//      // TODO log error
//    } */
// }

// llvm::Value *pache::binary_plus_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateAdd(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFAdd(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return nullptr;
//     // TODO log error
//   }
// }

// llvm::Value *pache::binary_minus_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateSub(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFSub(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return nullptr;
//     // TODO log error
//   }
// }

// llvm::Value *pache::three_way_exp::codegen() {
//   return nullptr;

//   // TODO
// }

// llvm::Value *pache::less_exp::codegen() {
//   /*  if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//      if (m_lhs->get_type()->is_signed()) {
//        return Builder->CreateICmpSLT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else if (m_lhs->get_type()->is_unsigned()) {
//        return Builder->CreateICmpULT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else {
//        return nullptr;
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFCmpOLT(m_lhs->codegen(), m_rhs->codegen(),
//                                    ""); // ordered and less than
//    } else {
//      return nullptr;
//      // TODO log error
//    } */
// }

// llvm::Value *pache::less_eq_exp::codegen() {
//   /*   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//       if (m_lhs->get_type()->is_signed()) {
//         return Builder->CreateICmpSLE(m_lhs->codegen(), m_rhs->codegen(),
//         "");
//       } else if (m_lhs->get_type()->is_unsigned()) {
//         return Builder->CreateICmpULE(m_lhs->codegen(), m_rhs->codegen(),
//         "");
//       } else {
//         return nullptr;
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFCmpOLE(m_lhs->codegen(), m_rhs->codegen(),
//                                     ""); // ordered and less than
//     } else {
//       return nullptr;
//       // TODO log error
//     } */
// }

// llvm::Value *pache::greater_exp::codegen() {
//   /*  if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//      if (m_lhs->get_type()->is_signed()) {
//        return Builder->CreateICmpSGT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else if (m_lhs->get_type()->is_unsigned()) {
//        return Builder->CreateICmpUGT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else {
//        return nullptr;
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFCmpOGT(m_lhs->codegen(), m_rhs->codegen(),
//                                    ""); // ordered and less than
//    } else {
//      return nullptr;
//      // TODO log error
//    } */
// }

// llvm::Value *pache::greater_eq_exp::codegen() {
//   /*   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//       if (m_lhs->get_type()->is_signed()) {
//         return Builder->CreateICmpSGE(m_lhs->codegen(), m_rhs->codegen(),
//         "");
//       } else if (m_lhs->get_type()->is_unsigned()) {
//         return Builder->CreateICmpUGE(m_lhs->codegen(), m_rhs->codegen(),
//         "");
//       } else {
//         return nullptr;
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFCmpOGE(m_lhs->codegen(), m_rhs->codegen(),
//                                     ""); // ordered and less than
//     } else {
//       return nullptr;
//       // TODO log error
//     } */
// }

// llvm::Value *pache::eq_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateICmpEQ(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFCmpOEQ(m_lhs->codegen(), m_rhs->codegen(),
//                                   ""); // ordered and less than
//   } else {
//     return nullptr;
//     // TODO log error
//   }
// }

// llvm::Value *pache::not_eq_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateICmpNE(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFCmpONE(m_lhs->codegen(), m_rhs->codegen(),
//                                   ""); // ordered and not equal
//   } else {
//     return nullptr;
//     // TODO log error
//   }
// }

// llvm::Value *pache::bitwise_and_exp::codegen() {
//   return Builder->CreateAnd(m_lhs->codegen(), m_rhs->codegen(),
//                             ""); // ordered ang greater than or equal
// }

// llvm::Value *pache::bitwise_xor_exp::codegen() {
//   return Builder->CreateXor(m_lhs->codegen(), m_rhs->codegen(),
//                             ""); // ordered and equal
// }

// llvm::Value *pache::bitwise_or_exp::codegen() {
//   return Builder->CreateOr(m_lhs->codegen(), m_rhs->codegen(),
//                            ""); // ordered and not equal
// }

// llvm::Value *pache::logical_and_exp::codegen() {
//   return Builder->CreateLogicalAnd(m_lhs->codegen(), m_rhs->codegen(), "");
// }

// llvm::Value *pache::logical_or_exp::codegen() {
//   return Builder->CreateLogicalOr(m_lhs->codegen(), m_rhs->codegen(), "");
//   ;
// }
