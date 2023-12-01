#include "operator.h"
#include "function.h"
#include "function_type.h"
#include "type.h"
#include <llvm/IR/Constants.h>
#include <llvm/IR/DerivedTypes.h>
#include <llvm/IR/Function.h>
#include <llvm/IR/Verifier.h>
#include <memory>
#include <vector>

namespace pache {

inline namespace __operator_unary_plus {
function_build make_operator_unary_plus_i8() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i8_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i8_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T2i8", TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T2i8"sv, std::move(type), F};
}

function_build make_operator_unary_plus_i16() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i16_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i16_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i16", TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i16"s, std::move(type), F};
}

function_build make_operator_unary_plus_i32() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i32_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i32_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i32"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i32"s, std::move(type), F};
}

function_build make_operator_unary_plus_i64() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i64_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i64_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i64"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i64"s, std::move(type), F};
}

function_build make_operator_unary_plus_i128() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i128_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i128_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4i128"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4i128"s, std::move(type), F};
}

function_build make_operator_unary_plus_u8() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<u8_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<u8_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T2u8"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T2u8"s, std::move(type), F};
}

function_build make_operator_unary_plus_u16() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<u16_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<u16_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u16"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u16"s, std::move(type), F};
}

function_build make_operator_unary_plus_u32() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<u32_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<u32_type_t>(),
      std::move(args_type));
      
  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u32"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u32"s, std::move(type), F};
}

function_build make_operator_unary_plus_u64() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<u64_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<u64_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u64"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u64"s, std::move(type), F};
}

function_build make_operator_unary_plus_u128() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<u128_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<u128_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4u128"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4u128"s, std::move(type), F};
}

function_build make_operator_unary_plus_f16() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f16_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f16_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f16"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f16"s, std::move(type), F};
}

function_build make_operator_unary_plus_f32() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f32_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f32_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f32"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f32"s, std::move(type), F};
}

function_build make_operator_unary_plus_f64() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f64_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f64_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f64"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f64"s, std::move(type), F};
}

function_build make_operator_unary_plus_f128() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f128_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f128_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4f128"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4f128"s, std::move(type), F};
}

} // namespace __operator_unary_plus

std::vector<function_build> const  operator_unary_plus = []{
  std::vector<function_build> val;
  val.emplace_back(make_operator_unary_plus_i8());
  val.emplace_back(make_operator_unary_plus_i16());
  val.emplace_back(make_operator_unary_plus_i32());
  val.emplace_back(make_operator_unary_plus_i64());
  val.emplace_back(make_operator_unary_plus_i128());
  val.emplace_back(make_operator_unary_plus_u8());
  val.emplace_back(make_operator_unary_plus_u16());
  val.emplace_back(make_operator_unary_plus_u32());
  val.emplace_back(make_operator_unary_plus_u64());
  val.emplace_back(make_operator_unary_plus_u128());
  val.emplace_back(make_operator_unary_plus_f16());
  val.emplace_back(make_operator_unary_plus_f32());
  val.emplace_back(make_operator_unary_plus_f64());
  val.emplace_back(make_operator_unary_plus_f128());

  val.shrink_to_fit();
  return val;
}();

inline namespace __operator_unary_minus {
function_build make_operator_unary_minus_i8() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i8_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i8_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T2i8"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(Builder->CreateSub(Builder->getInt8(0), F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T2i8"s, std::move(type), F};
}

function_build make_operator_unary_minus_i16() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i16_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i16_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3i16"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(
      Builder->CreateSub(Builder->getInt16(0), F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3i16"s, std::move(type), F};
}

function_build make_operator_unary_minus_i32() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i32_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i32_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3i32"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(
      Builder->CreateSub(Builder->getInt32(0), F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3i32"s, std::move(type), F};
}

function_build make_operator_unary_minus_i64() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i64_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i64_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3i64"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(
      Builder->CreateSub(Builder->getInt64(0), F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3i64"s, std::move(type), F};
}

function_build make_operator_unary_minus_i128() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<i128_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<i128_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T4i128"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(
      Builder->CreateSub(Builder->getIntN(128, 0), F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T4i128"s, std::move(type), F};
}

function_build make_operator_unary_minus_f16() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f16_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f16_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3f16"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);
  Builder->CreateRet(Builder->CreateFSub(
      llvm::ConstantFP::getZero(std::make_unique<f16_type_t>()->get_llvm_type()),
      F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3f16"s, std::move(type), F};
}

function_build make_operator_unary_minus_f32() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f32_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f32_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3f32"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(Builder->CreateFSub(
      llvm::ConstantFP::getZero(std::make_unique<f32_type_t>()->get_llvm_type()),
      F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3f32"s, std::move(type), F};
}

function_build make_operator_unary_minus_f64() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f64_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f64_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T3f64"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(Builder->CreateFSub(
      llvm::ConstantFP::getZero(std::make_unique<f64_type_t>()->get_llvm_type()),
      F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T3f64"s, std::move(type), F};
}

function_build make_operator_unary_minus_f128() {
  std::vector<std::unique_ptr<build_type>> args_type;
  args_type.emplace_back(std::make_unique<f128_type_t>());
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      std::make_unique<f128_type_t>(),
      std::move(args_type));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator-_T4f128"s, TheModule.get());

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(Builder->CreateFSub(
      llvm::ConstantFP::getZero(std::make_unique<f128_type_t>()->get_llvm_type()),
      F->getArg(0), ""));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator-_T4f128"s, std::move(type), F};
}

} // namespace __operator_unary_minus

std::vector<function_build> const operator_unary_minus = []{
  std::vector<function_build> val;
  val.emplace_back(make_operator_unary_minus_i8());
  val.emplace_back(make_operator_unary_minus_i16());
  val.emplace_back(make_operator_unary_minus_i32());
  val.emplace_back(make_operator_unary_minus_i64());
  val.emplace_back(make_operator_unary_minus_i128());
  val.emplace_back(make_operator_unary_minus_f16());
  val.emplace_back(make_operator_unary_minus_f32());
  val.emplace_back(make_operator_unary_plus_f64());
  val.emplace_back(make_operator_unary_minus_f128());

  val.shrink_to_fit();
  return val;
}();

} // namespace pache
