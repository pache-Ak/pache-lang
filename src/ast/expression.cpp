#include "expression.h"
#include "../IRbuild/expression.h"
#include <memory>
#include <iostream>

namespace pache {
exp_ast::~exp_ast(){}

std::unique_ptr<build_variable>
pache::unary_plus::build(base_build &build) const {
  return build_unary_plus(build, *this);
}

std::unique_ptr<build_variable>
pache::unary_minus::build(base_build &build) const {
  return build_unary_minus(build, *this);
}

std::unique_ptr<build_variable>
pache::func_call_exp::build(base_build &build) const {
  return build_func_call_exp(build, *this);
}

std::unique_ptr<build_variable> var_exp::build(base_build &build) const {
  return build_var_exp(build, *this);
}

std::unique_ptr<build_variable>
binary_mul_exp::build(base_build &build) const {
  return build_binary_mul_exp(build, *this);
}

std::unique_ptr<build_variable>
binary_div_exp::build(base_build &build) const {
  return build_binary_div_exp(build, *this);
}

std::unique_ptr<build_variable>
binary_mod_exp::build(base_build &build) const {
  return build_binary_mod_exp(build, *this);
}

std::unique_ptr<build_variable>
binary_plus_exp::build(base_build &build) const {
  return build_binary_plus_exp(build, *this);
}

std::unique_ptr<build_variable>
binary_minus_exp::build(base_build &build) const {
  return build_binary_minus_exp(build, *this);
}

std::unique_ptr<build_variable>
three_way_exp::build(base_build &build) const {
  return build_three_way_exp(build, *this);
}

std::unique_ptr<build_variable> less_exp::build(base_build &build) const {
  return build_less_exp(build, *this);
}

std::unique_ptr<build_variable>
less_eq_exp::build(base_build &build) const {
  return build_less_eq_exp(build, *this);
}

std::unique_ptr<build_variable>
greater_exp::build(base_build &build) const {
  return build_greater_exp(build, *this);
}

std::unique_ptr<build_variable>
greater_eq_exp::build(base_build &build) const {
  return build_greater_eq_exp(build, *this);
}

std::unique_ptr<build_variable> eq_exp::build(base_build &build) const {
  return build_eq_exp(build, *this);
}

std::unique_ptr<build_variable>
not_eq_exp::build(base_build &build) const {
  return build_not_eq_exp(build, *this);
}

std::unique_ptr<build_variable>
bitwise_and_exp::build(base_build &build) const {
  return build_bitwise_and_exp(build, *this);
}

std::unique_ptr<build_variable>
bitwise_xor_exp::build(base_build &build) const {
  return build_bitwise_xor_exp(build, *this);
}

std::unique_ptr<build_variable>
bitwise_or_exp::build(base_build &build) const {
  return build_bitwise_or_exp(build, *this);
}

std::unique_ptr<build_variable>
logical_and_exp::build(base_build &build) const {
  return build_logical_and_exp(build, *this);
}

std::unique_ptr<build_variable>
logical_or_exp::build(base_build &build) const {
  return build_logical_or_exp(build, *this);
}

std::unique_ptr<build_variable>
subscript_exp::build(base_build &build) const {
  return build_subscript_exp(build, *this);
}

std::unique_ptr<build_variable>
dot_exp::build(base_build &build) const {
  return build_dot_exp(build, *this);
}

std::unique_ptr<build_variable>
arrow_exp::build(base_build &build) const {
  // TODO return build_arrow_exp(build, *this);
}

std::unique_ptr<build_variable>
logical_not_exp::build(base_build &build) const {
  // TODO return build_logical_not_exp(build, *this);
}

std::unique_ptr<build_variable>
bitwise_not_exp::build(base_build &build) const {
  // TODO return build_bitwise_not_exp(build, *this);
}

std::unique_ptr<build_variable>
indirection_exp::build(base_build &build) const {
  // TODO return build_indirection_exp(build, *this);
}

std::unique_ptr<build_variable>
address_of_exp::build(base_build &build) const {
  // TODO return build_address_of_exp(build, *this);
}

std::unique_ptr<build_variable>
allocation_exp::build(base_build &build) const {
  // TODO return build_allocation_exp(build, *this);
}

std::unique_ptr<build_variable>
deallocation_exp::build(base_build &build) const {
  // TODO return build_deallocation_exp(build, *this);
}

std::unique_ptr<build_variable>
left_shift_exp::build(base_build &build) const {
  // TODO return build_left_shift_exp(build, *this);
}

std::unique_ptr<build_variable>
right_shift_exp::build(base_build &build) const {
  // TODO return build_right_shift_exp(build, *this);
}
void unary_plus::print() const {
  std::cout << "unary_plus:\n";
  m_argument->print();
}
void unary_minus::print() const {
  std::cout << "unary_minus:\n";
  m_argument->print();
}
std::vector<std::unique_ptr<exp_ast>> const &func_call_exp::get_args() const {
  return m_args;
}
exp_ast const &func_call_exp::get_func() const { return *m_exp; }
func_call_exp::func_call_exp(std::unique_ptr<exp_ast> &&func,
                             std::vector<std::unique_ptr<exp_ast>> &&args)
    : m_exp(std::move(func)), m_args(std::move(args)) {}
void func_call_exp::print() const {
  std::cout << "func_call_exp:\n"
            << "m_exp:\n";
  m_exp->print();
  std::cout << "m_args:{\n";
  for (auto const &ast : m_args) {
    ast->print();
  }
  std::cout << "}\n";
}
void var_exp::print() const {
  std::cout << "var_exp:\n" << m_iden.get_name() << "\n";
}
std::string_view var_exp::get_name() const { return m_iden.get_name(); }
void binary_mul_exp::print() const {
  std::cout << "binary_mul_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void binary_div_exp::print() const {
  std::cout << "binary_div_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void binary_mod_exp::print() const {
  std::cout << "binary_mod_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void binary_plus_exp::print() const {
  std::cout << "binary_plus_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void binary_minus_exp::print() const {
  std::cout << "binary_minus_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void three_way_exp::print() const {
  std::cout << "three_way_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void less_exp::print() const {
  std::cout << "less_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void less_eq_exp::print() const {
  std::cout << "less_eq_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void greater_exp::print() const {
  std::cout << "greater_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void greater_eq_exp::print() const {
  std::cout << "greater_eq_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void eq_exp::print() const {
  std::cout << "eq_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void not_eq_exp::print() const {
  std::cout << "not_eq_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void bitwise_and_exp::print() const {
  std::cout << "bitwise_and_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void bitwise_xor_exp::print() const {
  std::cout << "bitwise_xor_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void bitwise_or_exp::print() const {
  std::cout << "bitwise_or_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void logical_and_exp::print() const {
  std::cout << "logical_and_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void logical_or_exp::print() const {
  std::cout << "logical_or_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void subscript_exp::print() const {
  std::cout << "subscript_exp:\n"
            << "m_function:\n";
  m_function->print();
  std::cout << "m_rhs:{\n";
  for (auto const &ast : m_args) {
    ast->print();
  }
  std::cout << "}\n";
}
void dot_exp::print() const {
  std::cout << "dot_exp:\n"
            << "m_expr:\n";
  m_expr->print();
  std::cout << "m_id_expr:\n" << m_id_expr << "\n";
}
void arrow_exp::print() const {
  std::cout << "arrow_exp:\n"
            << "m_expr:\n";
  m_expr->print();
  std::cout << "m_id_expr:\n" << m_id_expr << "\n";
}
void bitwise_not_exp::print() const {
  std::cout << "bitwise_not_exp:\n"
            << "m_argument:\n";
  m_argument->print();
}
void address_of_exp::print() const {
  std::cout << "address_of_exp:\n"
            << "m_argument:\n";
  m_argument->print();
}
void allocation_exp::print() const {
  std::cout << "allocation_exp:\n"
            << "m_type:\n";
  m_type->print();
  std::cout << "m_init:\n";
  m_init->print();
}
void deallocation_exp::print() const {
  std::cout << "deallocation_exp:\n"
            << "m_exp:\n";
  m_exp->print();
}
void left_shift_exp::print() const {
  std::cout << "left_shift_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void right_shift_exp::print() const {
  std::cout << "right_shift_exp:\n"
            << "m_lhs:\n";
  m_lhs->print();
  std::cout << "m_rhs:\n";
  m_rhs->print();
}
void logical_not_exp::print() const {
  std::cout << "logical_not_exp:\n"
            << "m_argument:\n";
  m_argument->print();
}
void indirection_exp::print() const {
  std::cout << "indirection_exp:\n"
            << "m_argument:\n";
  m_argument->print();
}
} // namespace pache

// llvm::Value *pache::var_exp::codegen() {
//   /*   llvm::Value *val = get_father()->find_named_Value(m_name);
//     if (val == nullptr) {
//       // TODO log error
//     }

//     return val; */

//   return {};
// }

// llvm::Value *pache::binary_mul_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateMul(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFMul(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return {};
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
//         return {};
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFDiv(m_lhs->codegen(), m_rhs->codegen(), "");
//     } else {
//       return {};
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
//        return {};
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFRem(m_lhs->codegen(), m_rhs->codegen(), "");
//    } else {
//      return {};
//      // TODO log error
//    } */
// }

// llvm::Value *pache::binary_plus_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateAdd(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFAdd(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return {};
//     // TODO log error
//   }
// }

// llvm::Value *pache::binary_minus_exp::codegen() {
//   if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//     return Builder->CreateSub(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//     return Builder->CreateFSub(m_lhs->codegen(), m_rhs->codegen(), "");
//   } else {
//     return {};
//     // TODO log error
//   }
// }

// llvm::Value *pache::three_way_exp::codegen() {
//   return {};

//   // TODO
// }

// llvm::Value *pache::less_exp::codegen() {
//   /*  if (m_lhs->codegen()->getType()->isIntOrIntVectorTy()) {
//      if (m_lhs->get_type()->is_signed()) {
//        return Builder->CreateICmpSLT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else if (m_lhs->get_type()->is_unsigned()) {
//        return Builder->CreateICmpULT(m_lhs->codegen(), m_rhs->codegen(), "");
//      } else {
//        return {};
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFCmpOLT(m_lhs->codegen(), m_rhs->codegen(),
//                                    ""); // ordered and less than
//    } else {
//      return {};
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
//         return {};
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFCmpOLE(m_lhs->codegen(), m_rhs->codegen(),
//                                     ""); // ordered and less than
//     } else {
//       return {};
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
//        return {};
//        // TODO log error;
//      }
//    } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//      return Builder->CreateFCmpOGT(m_lhs->codegen(), m_rhs->codegen(),
//                                    ""); // ordered and less than
//    } else {
//      return {};
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
//         return {};
//         // TODO log error;
//       }
//     } else if (m_lhs->codegen()->getType()->isFPOrFPVectorTy()) {
//       return Builder->CreateFCmpOGE(m_lhs->codegen(), m_rhs->codegen(),
//                                     ""); // ordered and less than
//     } else {
//       return {};
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
//     return {};
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
//     return {};
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
