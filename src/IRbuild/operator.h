#ifndef IRBUILD_OPERATOR_H
#define IRBUILD_OPERATOR_H

#include "function.h"
#include "variable.h"
#include "type.h"
#include <llvm-17/llvm/IR/Value.h>
#include <memory>
#include <string_view>
#include <unordered_map>
#include <vector>

namespace pache {
extern std::unordered_multimap<std::string_view, std::unique_ptr<build_variable>> const primer_functions;

llvm::Value *zext_or_trunc(build_variable &value, llvm::Type *destty);
llvm::Value *sext_or_trunc(build_variable &value, llvm::Type *destty);


extern std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&)
> const operator_unary_plus_map;  // +a
extern std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&)
> const operator_unary_minus_map;  // -a

extern std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_plus_map;  // a + b
extern std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_minus_map;      // a - b
extern std::unordered_map<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_mul_map;        // a * b
extern std::unordered_multimap<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_div_map;        // a / b
extern std::unordered_multimap<
    type::type_ID,
    llvm::Value* (*)(build_variable&, build_variable&)
> const operator_binary_mod_map;        // a % b



} // namespace pache

#endif
