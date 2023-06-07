
#include "type.h"
#include "ast.h"
#include "compunit.h"

namespace pache {
void_type_t void_type_t::void_type{};

bool_type_t bool_type_t::bool_type{};

size_type_t size_type_t::size_type{};

i8_type_t i8_type_t::i8_type{};
i16_type_t i16_type_t::i16_type{};
i32_type_t i32_type_t::i32_type{};
i64_type_t i64_type_t::i64_type{};
i128_type_t i128_type_t::i128_type{};

u8_type_t u8_type_t::u8_type{};
u16_type_t u16_type_t::u16_type{};
u32_type_t u32_type_t::u32_type{};
u64_type_t u64_type_t::u64_type{};
u128_type_t u128_type_t::u128_type{};

f16_type_t f16_type_t::f16_type{};
f32_type_t f32_type_t::f32_type{};
f64_type_t f64_type_t::f64_type{};
f128_type_t f128_type_t::f128_type{};

d32_type_t d32_type_t::d32_type{};
d64_type_t d64_type_t::d64_type{};

c8_type_t c8_type_t::c8_type{};
c16_type_t c16_type_t::c16_type{};
c32_type_t c32_type_t::c32_type{};

i8_type_t *pache::i8_type_t::get() { return &i8_type; }

i16_type_t *pache::i16_type_t::get() { return &i16_type; }

i32_type_t *pache::i32_type_t::get() { return &i32_type; }

i64_type_t *pache::i64_type_t::get() { return &i64_type; }

i128_type_t *pache::i128_type_t::get() { return &i128_type; }

u8_type_t *pache::u8_type_t::get() { return &u8_type; }

u16_type_t *pache::u16_type_t::get() { return &u16_type; }

u32_type_t *pache::u32_type_t::get() { return &u32_type; }

u64_type_t *pache::u64_type_t::get() { return &u64_type; }

u128_type_t *pache::u128_type_t::get() { return &u128_type; }

f16_type_t *pache::f16_type_t::get() { return &f16_type; }

f32_type_t *pache::f32_type_t::get() { return &f32_type; }

f64_type_t *pache::f64_type_t::get() { return &f64_type; }

f128_type_t *pache::f128_type_t::get() { return &f128_type; }

d32_type_t *pache::d32_type_t::get() { return &d32_type; }

d64_type_t *pache::d64_type_t::get() { return &d64_type; }

c8_type_t *pache::c8_type_t::get() { return &c8_type; }

c16_type_t *pache::c16_type_t::get() { return &c16_type; }

c32_type_t *pache::c32_type_t::get() { return &c32_type; }

void_type_t *pache::void_type_t::get() { return &void_type; }

bool_type_t *pache::bool_type_t::get() { return &bool_type; }

size_type_t *pache::size_type_t::get() { return &size_type; }

func_type::func_type(const std::vector<variable_ast *> &args,
                     const type_ast *return_type)
    : m_return_type(return_type) {
  std::for_each(args.begin(), args.end(), [=](variable_ast *var) {
    m_args_type.push_back(var->get_type());
  });
}

type_ast *user_def_type::get(compunit_ast *comp) {
  /* return comp->find_class_def(m_iden)
              ? new class_type(comp->find_class_def(m_iden))
              : nullptr;*/
}
} // namespace pache

llvm::Type *pache::i8_type_t::codegen() {
  return llvm::Type::getInt8Ty(*TheContext);
}

llvm::Type *pache::i16_type_t::codegen() {
  return llvm::Type::getInt16Ty(*TheContext);
}

llvm::Type *pache::i32_type_t::codegen() {
  return llvm::Type::getInt32Ty(*TheContext);
}

llvm::Type *pache::i64_type_t::codegen() {
  return llvm::Type::getInt64Ty(*TheContext);
}

llvm::Type *pache::i128_type_t::codegen() {
  return llvm::Type::getInt128Ty(*TheContext);
}

llvm::Type *pache::u8_type_t::codegen() {
  return llvm::Type::getInt8Ty(*TheContext);
}

llvm::Type *pache::u16_type_t::codegen() {
  return llvm::Type::getInt16Ty(*TheContext);
}

llvm::Type *pache::u32_type_t::codegen() {
  return llvm::Type::getInt32Ty(*TheContext);
}

llvm::Type *pache::u64_type_t::codegen() {
  return llvm::Type::getInt64Ty(*TheContext);
}

llvm::Type *pache::u128_type_t::codegen() {
  return llvm::Type::getInt128Ty(*TheContext);
}

llvm::Type *pache::f16_type_t::codegen() {
  return llvm::Type::getHalfTy(*TheContext);
}

llvm::Type *pache::f32_type_t::codegen() {
  return llvm::Type::getFloatTy(*TheContext);
}

llvm::Type *pache::f64_type_t::codegen() {
  return llvm::Type::getDoubleTy(*TheContext);
}

llvm::Type *pache::f128_type_t::codegen() {
  return llvm::Type::getFP128Ty(*TheContext);
}

llvm::Type *pache::d32_type_t::codegen() {
  return nullptr; // TODO
}

llvm::Type *pache::d64_type_t::codegen() {
  return nullptr; // TODO
}

llvm::Type *pache::c8_type_t::codegen() {
  return llvm::Type::getInt8Ty(*TheContext);
}

llvm::Type *pache::c16_type_t::codegen() {
  return llvm::Type::getInt16Ty(*TheContext);
}

llvm::Type *pache::c32_type_t::codegen() {
  return llvm::Type::getInt32Ty(*TheContext);
}

llvm::Type *pache::void_type_t::codegen() {
  return llvm::Type::getVoidTy(*TheContext);
}

llvm::Type *pache::bool_type_t::codegen() {
  return llvm::Type::getInt1Ty(*TheContext);
}

llvm::Type *pache::size_type_t::codegen() {
  return llvm::Type::getInt64Ty(
      *TheContext); // size's type should more consider
}

llvm::Type *pache::arr_type_t::codegen() {
  return llvm::ArrayType::get(m_element_type->codegen(), m_size);
}

llvm::Type *pache::class_type::codegen() {
  return nullptr; // TODO
}

llvm::Type *pache::user_def_type::codegen() {
  return nullptr; /// TODO
}