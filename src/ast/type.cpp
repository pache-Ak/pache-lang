#include "type.h"
#include "../IRbuild/type.h"
#include "../IRbuild/function_type.h"

namespace pache {
func_type_ast::func_type_ast(std::vector<std::unique_ptr<type_ast>> &&args,
                     std::unique_ptr<type_ast> &&return_type)
    : m_args_type(std::move(args)), m_return_type(std::move(return_type)) {}

std::unique_ptr<build_type> const_ast::build(base_build &father) const {
  return build_const_type(father, *this);
}

std::unique_ptr<build_type> volatile_ast::build(base_build &father) const {
  return build_volatile_type(father, *this);
}

std::unique_ptr<build_type> reference_ast::build(base_build &father) const {
  return build_reference_type(father, *this);
}

std::unique_ptr<build_type> pointer_ast::build(base_build &father) const {
  return build_pointer_type(father, *this);
}

std::unique_ptr<build_type> arr_ast::build(base_build &father) const {
  return build_arr_type(father, *this);
}

std::unique_ptr<build_type> multi_array_ast::build(base_build &father) const {
  return build_multi_array_type(father, *this);
}

std::unique_ptr<build_type> func_type_ast::build(base_build &father) const {
  return build_func_type(father, *this);
}

std::unique_ptr<build_type> named_ast::build(base_build &father) const {
  return build_named_type(father, *this);
}
/* std::unique_ptr<build_type> void_type_ast::build(base_build &father) const {
  return void_type_t::get();
}

std::unique_ptr<type_ast> void_type_ast::get() {
  return std::make_unique<void_type_ast>();
} */
// void_type_t void_type_t::void_type{};

// bool_type_t bool_type_t::bool_type{};

// size_type_t size_type_t::size_type{};

// i8_type_t i8_type_t::i8_type{};
// i16_type_t i16_type_t::i16_type{};
// i32_type_t i32_type_t::i32_type{};
// i64_type_t i64_type_t::i64_type{};
// i128_type_t i128_type_t::i128_type{};

// u8_type_t u8_type_t::u8_type{};
// u16_type_t u16_type_t::u16_type{};
// u32_type_t u32_type_t::u32_type{};
// u64_type_t u64_type_t::u64_type{};
// u128_type_t u128_type_t::u128_type{};

// f16_type_t f16_type_t::f16_type{};
// f32_type_t f32_type_t::f32_type{};
// f64_type_t f64_type_t::f64_type{};
// f128_type_t f128_type_t::f128_type{};

// d32_type_t d32_type_t::d32_type{};
// d64_type_t d64_type_t::d64_type{};

// c8_type_t c8_type_t::c8_type{};
// c16_type_t c16_type_t::c16_type{};
// c32_type_t c32_type_t::c32_type{};

// std::unique_ptr<pache::type_ast> pache::i8_type_t::get() {
//   return std::unique_ptr<pache::i8_type_t>(&i8_type);
// }

// std::unique_ptr<pache::type_ast> pache::i16_type_t::get() {
//   return std::unique_ptr<i16_type_t>(&i16_type);
// }

// std::unique_ptr<pache::type_ast> pache::i32_type_t::get() {
//   return std::unique_ptr<i32_type_t>(&i32_type);
// }

// std::unique_ptr<pache::type_ast> pache::i64_type_t::get() {
//   return std::unique_ptr<i64_type_t>(&i64_type);
// }

// std::unique_ptr<pache::type_ast> pache::i128_type_t::get() {
//   return std::unique_ptr<i128_type_t>(&i128_type);
// }

// std::unique_ptr<pache::type_ast> pache::u8_type_t::get() {
//   return std::unique_ptr<u8_type_t>(&u8_type);
// }

// std::unique_ptr<pache::type_ast> pache::u16_type_t::get() {
//   return std::unique_ptr<u16_type_t>(&u16_type);
// }

// std::unique_ptr<pache::type_ast> pache::u32_type_t::get() {
//   return std::unique_ptr<u32_type_t>(&u32_type);
// }

// std::unique_ptr<pache::type_ast> pache::u64_type_t::get() {
//   return std::unique_ptr<u64_type_t>(&u64_type);
// }

// std::unique_ptr<pache::type_ast> pache::u128_type_t::get() {
//   return std::unique_ptr<u128_type_t>(&u128_type);
// }

// std::unique_ptr<pache::type_ast> pache::f16_type_t::get() {
//   return std::unique_ptr<f16_type_t>(&f16_type);
// }

// std::unique_ptr<pache::type_ast> pache::f32_type_t::get() {
//   return std::unique_ptr<f32_type_t>(&f32_type);
// }

// std::unique_ptr<pache::type_ast> pache::f64_type_t::get() {
//   return std::unique_ptr<f64_type_t>(&f64_type);
// }

// std::unique_ptr<pache::type_ast> pache::f128_type_t::get() {
//   return std::unique_ptr<f128_type_t>(&f128_type);
// }

// std::unique_ptr<pache::type_ast> pache::d32_type_t::get() {
//   return std::unique_ptr<d32_type_t>(&d32_type);
// }

// std::unique_ptr<pache::type_ast> pache::d64_type_t::get() {
//   return std::unique_ptr<d64_type_t>(&d64_type);
// }

// std::unique_ptr<pache::type_ast> pache::c8_type_t::get() {
//   return std::unique_ptr<c8_type_t>(&c8_type);
// }

// std::unique_ptr<pache::type_ast> pache::c16_type_t::get() {
//   return std::unique_ptr<c16_type_t>(&c16_type);
// }

// std::unique_ptr<pache::type_ast> pache::c32_type_t::get() {
//   return std::unique_ptr<c32_type_t>(&c32_type);
// }

// std::unique_ptr<pache::type_ast> pache::void_type_t::get() {
//   return std::unique_ptr<void_type_t>(&void_type);
// }

// std::unique_ptr<pache::type_ast> pache::bool_type_t::get() {
//   return std::unique_ptr<bool_type_t>(&bool_type);
// }

// std::unique_ptr<pache::type_ast> pache::size_type_t::get() {
//   return std::unique_ptr<size_type_t>(&size_type);
// }





} // namespace pache

// llvm::Type *pache::i8_type_t::codegen() {
//   return llvm::Type::getInt8Ty(*TheContext);
// }

// llvm::Type *pache::i16_type_t::codegen() {
//   return llvm::Type::getInt16Ty(*TheContext);
// }

// llvm::Type *pache::i32_type_t::codegen() {
//   return llvm::Type::getInt32Ty(*TheContext);
// }

// llvm::Type *pache::i64_type_t::codegen() {
//   return llvm::Type::getInt64Ty(*TheContext);
// }

// llvm::Type *pache::i128_type_t::codegen() {
//   return llvm::Type::getInt128Ty(*TheContext);
// }

// llvm::Type *pache::u8_type_t::codegen() {
//   return llvm::Type::getInt8Ty(*TheContext);
// }

// llvm::Type *pache::u16_type_t::codegen() {
//   return llvm::Type::getInt16Ty(*TheContext);
// }

// llvm::Type *pache::u32_type_t::codegen() {
//   return llvm::Type::getInt32Ty(*TheContext);
// }

// llvm::Type *pache::u64_type_t::codegen() {
//   return llvm::Type::getInt64Ty(*TheContext);
// }

// llvm::Type *pache::u128_type_t::codegen() {
//   return llvm::Type::getInt128Ty(*TheContext);
// }

// llvm::Type *pache::f16_type_t::codegen() {
//   return llvm::Type::getHalfTy(*TheContext);
// }

// llvm::Type *pache::f32_type_t::codegen() {
//   return llvm::Type::getFloatTy(*TheContext);
// }

// llvm::Type *pache::f64_type_t::codegen() {
//   return llvm::Type::getDoubleTy(*TheContext);
// }

// llvm::Type *pache::f128_type_t::codegen() {
//   return llvm::Type::getFP128Ty(*TheContext);
// }

// llvm::Type *pache::d32_type_t::codegen() {
//   return nullptr; // TODO
// }

// llvm::Type *pache::d64_type_t::codegen() {
//   return nullptr; // TODO
// }

// llvm::Type *pache::c8_type_t::codegen() {
//   return llvm::Type::getInt8Ty(*TheContext);
// }

// llvm::Type *pache::c16_type_t::codegen() {
//   return llvm::Type::getInt16Ty(*TheContext);
// }

// llvm::Type *pache::c32_type_t::codegen() {
//   return llvm::Type::getInt32Ty(*TheContext);
// }

// llvm::Type *pache::void_type_t::codegen() {
//   return llvm::Type::getVoidTy(*TheContext);
// }

// llvm::Type *pache::bool_type_t::codegen() {
//   return llvm::Type::getInt1Ty(*TheContext);
// }

// llvm::Type *pache::size_type_t::codegen() {
//   return llvm::Type::getInt64Ty(
//       *TheContext); // size's type should more consider
// }

// llvm::Type *pache::arr_type_t::codegen() {
//   return llvm::ArrayType::get(m_element_type->codegen(), m_size);
// }

// llvm::Type *pache::class_type::codegen() {
//   return nullptr; // TODO
// }

// llvm::Type *pache::user_def_type::codegen() {
//   return nullptr; /// TODO
// }
