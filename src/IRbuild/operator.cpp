#include "operator.h"
#include "function.h"
#include "function_type.h"
#include "type.h"
#include "variable.h"
#include <array>
#include <llvm-17/llvm/IR/Instructions.h>
#include <llvm-17/llvm/IR/Type.h>
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>
#include <iostream>

namespace pache {
llvm::Value *zext(build_variable &value, llvm::Type *destty) {
  std::cout << "zext to ty\n";
  return Builder->CreateZExt(value.get_value(), Builder->getInt32Ty());
}

llvm::Value *sext(build_variable &value, llvm::Type *destty) {
  std::cout << "sext to ty\n";
  return Builder->CreateSExt(value.get_value(), Builder->getInt32Ty());
}

llvm::Value *zext_or_trunc(build_variable &value, llvm::Type *destty) {
  std::cout << "zext or truncto ty\n";
  return Builder->CreateZExtOrTrunc(value.get_value(), Builder->getInt32Ty());
}

llvm::Value *sext_or_trunc(build_variable &value, llvm::Type *destty) {
  std::cout << "sext or trunc to ty\n";
  return Builder->CreateSExtOrTrunc(value.get_value(), Builder->getInt32Ty());
}

inline namespace __operator_unary_plus {
  // 不需要提升的类型
llvm::Value * operator_unary_plus(build_variable &value) {
  return value.get_value();
}

// 用于最小32位寄存器平台
llvm::Value * operator_unary_plus_z32(build_variable &value) {
  std::cout << "zext to i32\n";
  return Builder->CreateZExt(value.get_value(), Builder->getInt32Ty());
}

llvm::Value * operator_unary_plus_s32(build_variable &value) {
  std::cout << "sext to i32\n";
  return Builder->CreateSExt(value.get_value(), Builder->getInt32Ty());
}

llvm::Value * operator_unary_plus_f32(build_variable &value) {
  return Builder->CreateFPExt(value.get_value(), Builder->getFloatTy());
}
// 用于最小64位寄存器平台
llvm::Value * operator_unary_plus_z64(build_variable &value) {
  return Builder->CreateZExt(value.get_value(), Builder->getInt64Ty());
}

llvm::Value * operator_unary_plus_s64(build_variable &value) {
  return Builder->CreateSExt(value.get_value(), Builder->getInt64Ty());
}
} // namespace __operator_unary_plus

// 当前实现匹配最小支持32位寄存器平台
std::unordered_map<type::type_ID, llvm::Value* (*)(build_variable&)> const operator_unary_plus_map {
  {type::type_ID::I8, operator_unary_plus_s32},
  {type::type_ID::I16, operator_unary_plus_s32},
  {type::type_ID::I32, operator_unary_plus},
  {type::type_ID::I64, operator_unary_plus},
  //{type::type_ID::I128, operator_unary_plus},

  {type::type_ID::U8, operator_unary_plus_z32},
  {type::type_ID::U16, operator_unary_plus_z32},
  {type::type_ID::U32, operator_unary_plus},
  {type::type_ID::U64, operator_unary_plus},
  //{type::type_ID::U128, operator_unary_plus},

  {type::type_ID::F16, operator_unary_plus_f32},
  {type::type_ID::F32, operator_unary_plus},
  {type::type_ID::F64, operator_unary_plus},
  {type::type_ID::F128, operator_unary_plus},

};

inline namespace __operator_unary_minus {
llvm::Value * operator_unary_minus(build_variable &value) {
  return Builder->CreateNeg(value.get_value());
}  
template<class Arithmetic>
llvm::Value * operator_unary_minus_i(build_variable &value) {
  return Builder->CreateSub(
        llvm::ConstantAggregateZero::get(Arithmetic{}.get_llvm_type()),
        value.get_value());
}

template<class Arithmetic>
llvm::Value * operator_unary_minus_f(build_variable &value) {
  return Builder->CreateFSub(
        llvm::ConstantAggregateZero::get(Arithmetic{}.get_llvm_type()),
        value.get_value());
}
} // namespace __operator_unary_minus

std::unordered_map<type::type_ID, llvm::Value* (*)(build_variable&)> const operator_unary_minus_map {
  {type::type_ID::I8, operator_unary_minus},
  {type::type_ID::I16, operator_unary_minus},
  {type::type_ID::I32, operator_unary_minus},
  {type::type_ID::I64, operator_unary_minus},
  //{type::type_ID::I128, operator_unary_minus},

  {type::type_ID::F16, operator_unary_minus},
  {type::type_ID::F32, operator_unary_minus},
  {type::type_ID::F64, operator_unary_minus},
  {type::type_ID::F128, operator_unary_minus},
};

inline namespace __operator_binary_plus {


llvm::Value * operator_binary_plus_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateAdd(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_plus_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFAdd(lhs.get_value(), rhs.get_value());
}
}

std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_plus_map {
  {type::type_ID::I8, operator_binary_plus_i},
  {type::type_ID::I16, operator_binary_plus_i},
  {type::type_ID::I32, operator_binary_plus_i},
  {type::type_ID::I64, operator_binary_plus_i},
  //{type::type_ID::I128, operator_binary_plus_i},

  {type::type_ID::U8, operator_binary_plus_i},
  {type::type_ID::U16, operator_binary_plus_i},
  {type::type_ID::U32, operator_binary_plus_i},
  {type::type_ID::U64, operator_binary_plus_i},
  //{type::type_ID::U128, operator_binary_plus_i},

  {type::type_ID::F16, operator_binary_plus_f},
  {type::type_ID::F32, operator_binary_plus_f},
  {type::type_ID::F64, operator_binary_plus_f},
  {type::type_ID::F128, operator_binary_plus_f},
};


inline namespace __operator_binary_minus {
llvm::Value * operator_binary_minus_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateSub(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_minus_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFSub(lhs.get_value(), rhs.get_value());
}
}

std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_minus_map {
  {type::type_ID::I8, operator_binary_minus_i},
  {type::type_ID::I16, operator_binary_minus_i},
  {type::type_ID::I32, operator_binary_minus_i},
  {type::type_ID::I64, operator_binary_minus_i},
  //{type::type_ID::I128, operator_binary_minus_i},

  {type::type_ID::F16, operator_binary_minus_f},
  {type::type_ID::F32, operator_binary_minus_f},
  {type::type_ID::F64, operator_binary_minus_f},
  {type::type_ID::F128, operator_binary_minus_f},
};

inline namespace __operator_binary_mul {
llvm::Value * operator_binary_mul_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateMul(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_mul_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFMul(lhs.get_value(), rhs.get_value());
}
}

std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_mul_map {
  {type::type_ID::I8, operator_binary_mul_i},
  {type::type_ID::I16, operator_binary_mul_i},
  {type::type_ID::I32, operator_binary_mul_i},
  {type::type_ID::I64, operator_binary_mul_i},
  //{type::type_ID::I128, operator_binary_mul_i},

  {type::type_ID::U8, operator_binary_mul_i},
  {type::type_ID::U16, operator_binary_mul_i},
  {type::type_ID::U32, operator_binary_mul_i},
  {type::type_ID::U64, operator_binary_mul_i},
  //{type::type_ID::U128, operator_binary_mul_i},

  {type::type_ID::F16, operator_binary_mul_f},
  {type::type_ID::F32, operator_binary_mul_f},
  {type::type_ID::F64, operator_binary_mul_f},
  {type::type_ID::F128, operator_binary_mul_f},
};

inline namespace __operator_binary_div {
llvm::Value * operator_binary_div_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateSDiv(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_div_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateUDiv(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_div_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFDiv(lhs.get_value(), rhs.get_value());
}
}

std::unordered_multimap<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_div_map {
  {type::type_ID::I8, operator_binary_div_i},
  {type::type_ID::I16, operator_binary_div_i},
  {type::type_ID::I32, operator_binary_div_i},
  {type::type_ID::I64, operator_binary_div_i},
  //{type::type_ID::I128, operator_binary_div_i},

  {type::type_ID::U8, operator_binary_div_u},
  {type::type_ID::U16, operator_binary_div_u},
  {type::type_ID::U32, operator_binary_div_u},
  {type::type_ID::U64, operator_binary_div_u},
  //{type::type_ID::U128, operator_binary_div_u},

  {type::type_ID::F16, operator_binary_div_f},
  {type::type_ID::F32, operator_binary_div_f},
  {type::type_ID::F64, operator_binary_div_f},
  {type::type_ID::F128, operator_binary_div_f},
};

inline namespace __operator_binary_mod {
llvm::Value * operator_binary_mod_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateSRem(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_mod_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateURem(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_binary_mod_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFRem(lhs.get_value(), rhs.get_value());
}
}

// TODO 浮点取余运算需要定义

std::unordered_multimap<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_mod_map {
  {type::type_ID::I8, operator_binary_mod_i},
  {type::type_ID::I16, operator_binary_mod_i},
  {type::type_ID::I32, operator_binary_mod_i},
  {type::type_ID::I64, operator_binary_mod_i},
  //{type::type_ID::I128, operator_binary_mod_i},

  {type::type_ID::U8, operator_binary_mod_u},
  {type::type_ID::U16, operator_binary_mod_u},
  {type::type_ID::U32, operator_binary_mod_u},
  {type::type_ID::U64, operator_binary_mod_u},
  //{type::type_ID::U128, operator_binary_mod_u},

  {type::type_ID::F16, operator_binary_mod_f},
  {type::type_ID::F32, operator_binary_mod_f},
  {type::type_ID::F64, operator_binary_mod_f},
  {type::type_ID::F128, operator_binary_mod_f},
};

inline namespace __operator_logical_not {
llvm::Value * operator_logical_not(build_variable &value) {
  return Builder->CreateNot(value.get_value());
}
}

inline namespace __operator_logical_and {
llvm::Value * operator_logical_and(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateLogicalAnd(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_logical_or {
llvm::Value * operator_logical_or(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateLogicalOr(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_EQ {
llvm::Value * operator_EQ_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpEQ(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_EQ_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpOEQ(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_NE {
llvm::Value * operator_NE_i(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpNE(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_NE_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpONE(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_GT {
llvm::Value * operator_GT_s(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpSGT(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_GT_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpUGT(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_GT_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpOGT(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_GE {
llvm::Value * operator_GE_s(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpSGE(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_GE_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpUGE(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_GE_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpOGE(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_LT {
llvm::Value * operator_LT_s(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpSLT(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_LT_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpULT(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_LT_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpOLT(lhs.get_value(), rhs.get_value());
}
}

inline namespace __operator_LE {
llvm::Value * operator_LE_s(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpSLE(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_LE_u(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateICmpULE(lhs.get_value(), rhs.get_value());
}

llvm::Value * operator_LE_f(build_variable &lhs, build_variable &rhs) {
  return Builder->CreateFCmpOLE(lhs.get_value(), rhs.get_value());
}
}
inline namespace __operator_left_shift {
template<class lhs, class rhs>
function_build
make_operator_left_shift() {
//  std::vector<std::unique_ptr<build_type>> args_type;
 // args_type.emplace_back(std::make_unique<lhs>());
  std::unique_ptr<build_type>  type_ptr{std::make_unique<function_type>(
      std::make_unique<lhs>(),
      //std::move(args_type))
      {std::make_unique<lhs>(), std::make_unique<rhs>()})
  };

  function_type &type{static_cast<function_type &>(*type_ptr)};

    std::string name{name_mangling(type)};

  llvm::Function *F = llvm::Function::Create(
      type.get_llvm_type(), llvm::Function::ExternalLinkage,
      name, TheModule.get());

  std::vector<
    std::pair<std::string_view,
    std::unique_ptr<build_variable>
  >> var;
  unsigned Idx = 0;
  for (auto &Arg : F->args()) {
    llvm::AllocaInst *alloca = Builder->CreateAlloca(type.get_args_type()[Idx]->get_llvm_type()
        , nullptr, std::array<std::string_view, 1>{"arg"}[Idx]);
    Builder->CreateStore(&Arg, alloca);
    var.emplace_back(std::array<std::string_view, 1>{"arg"}[Idx],
        std::make_unique<build_local_variable>(type.get_args_type()[Idx]->clone(), alloca));
    ++Idx; 
  }
}
}
} // namespace pache
