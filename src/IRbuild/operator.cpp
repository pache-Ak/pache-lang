#include "operator.h"
#include "function.h"
#include "function_type.h"
#include "type.h"
#include <array>
#include <llvm-14/llvm/IR/DerivedTypes.h>
#include <llvm-14/llvm/IR/Function.h>
#include <llvm-14/llvm/IR/Verifier.h>
#include <memory>
#include <vector>

namespace pache {
inline namespace __operator_unary_plus {
function_build make_operator_unary_plus_i8() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      i8_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{i8_type_t::get()},
      llvm::FunctionType::get(i8_type_t::get()->get_llvm_type(),
                              {i8_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T2i8"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T2i8"s, std::move(type), F};
}

function_build make_operator_unary_plus_i16() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      i16_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{i16_type_t::get()},
      llvm::FunctionType::get(i16_type_t::get()->get_llvm_type(),
                              {i16_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i16"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i16"s, std::move(type), F};
}

function_build make_operator_unary_plus_i32() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      i32_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{i32_type_t::get()},
      llvm::FunctionType::get(i32_type_t::get()->get_llvm_type(),
                              {i32_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i32"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i32"s, std::move(type), F};
}

function_build make_operator_unary_plus_i64() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      i64_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{i64_type_t::get()},
      llvm::FunctionType::get(i64_type_t::get()->get_llvm_type(),
                              {i64_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3i64"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3i64"s, std::move(type), F};
}

function_build make_operator_unary_plus_i128() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      i128_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{i128_type_t::get()},
      llvm::FunctionType::get(i128_type_t::get()->get_llvm_type(),
                              {i128_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4i128"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4i128"s, std::move(type), F};
}

function_build make_operator_unary_plus_u8() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      u8_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{u8_type_t::get()},
      llvm::FunctionType::get(u8_type_t::get()->get_llvm_type(),
                              {u8_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T2u8"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T2u8"s, std::move(type), F};
}

function_build make_operator_unary_plus_u16() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      u16_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{u16_type_t::get()},
      llvm::FunctionType::get(u16_type_t::get()->get_llvm_type(),
                              {u16_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u16"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u16"s, std::move(type), F};
}

function_build make_operator_unary_plus_u32() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      u32_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{u32_type_t::get()},
      llvm::FunctionType::get(u32_type_t::get()->get_llvm_type(),
                              {u32_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u32"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u32"s, std::move(type), F};
}

function_build make_operator_unary_plus_u64() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      u64_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{u64_type_t::get()},
      llvm::FunctionType::get(u64_type_t::get()->get_llvm_type(),
                              {u64_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3u64"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3u64"s, std::move(type), F};
}

function_build make_operator_unary_plus_u128() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      u128_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{u128_type_t::get()},
      llvm::FunctionType::get(u128_type_t::get()->get_llvm_type(),
                              {u128_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4u128"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4u128"s, std::move(type), F};
}

function_build make_operator_unary_plus_f16() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      f16_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{f16_type_t::get()},
      llvm::FunctionType::get(f16_type_t::get()->get_llvm_type(),
                              {f16_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f16"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f16"s, std::move(type), F};
}

function_build make_operator_unary_plus_f32() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      f32_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{f32_type_t::get()},
      llvm::FunctionType::get(f32_type_t::get()->get_llvm_type(),
                              {f32_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f32"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f32"s, std::move(type), F};
}

function_build make_operator_unary_plus_f64() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      f64_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{f64_type_t::get()},
      llvm::FunctionType::get(f64_type_t::get()->get_llvm_type(),
                              {f64_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T3f64"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T3f64"s, std::move(type), F};
}

function_build make_operator_unary_plus_f128() {
  std::unique_ptr<function_type> type = std::make_unique<function_type>(
      f128_type_t::get(),
      std::vector<std::unique_ptr<build_type>>{f128_type_t::get()},
      llvm::FunctionType::get(f128_type_t::get()->get_llvm_type(),
                              {f128_type_t::get()->get_llvm_type()}, false));

  llvm::Function *F = llvm::Function::Create(
      type->get_llvm_type(), llvm::Function::ExternalLinkage,
      "_O9operator+_T4f128"s, TheModule.get());

  /*   for (auto &Arg : F->args()) {
      Arg.setName("self");
    } */

  llvm::BasicBlock *BB = llvm::BasicBlock::Create(*TheContext, "", F);
  Builder->SetInsertPoint(BB);

  Builder->CreateRet(F->getArg(0));
  llvm::verifyFunction(*F);

  return function_build{nullptr, "_O9operator+_T4f128"s, std::move(type), F};
}

} // namespace __operator_unary_plus

std::vector<function_build> operator_unary_plus{
    make_operator_unary_plus_i8(),   make_operator_unary_plus_i16(),
    make_operator_unary_plus_i32(),  make_operator_unary_plus_i64(),
    make_operator_unary_plus_i128(), make_operator_unary_plus_u8(),
    make_operator_unary_plus_u16(),  make_operator_unary_plus_u32(),
    make_operator_unary_plus_u64(),  make_operator_unary_plus_u128(),
    make_operator_unary_plus_f16(),  make_operator_unary_plus_f32(),
    make_operator_unary_plus_f64(),  make_operator_unary_plus_f128(),

};
} // namespace pache
