#include "type.h"
#include "build.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <functional>
#include <iostream>
#include <memory>
#include <numeric>
#include <string>

using namespace std::literals;

namespace pache {
bool build_type::is_integral() const { return false; }
bool build_type::is_signed() const { return false; }
bool build_type::is_unsigned() const { return false; }
std::string const build_type::decorated_name() const { return ""s; }
bool build_type::is_const() const { return m_is_const; }
bool build_type::is_volatile() const { return m_is_volatile; }

void primary_type::set_const() {
  if (is_const()) {
    std::cerr << "redef const.\n";
  } else {
    m_is_const = true;
  }
}
void primary_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}

void *void_type_t::operator new(std::size_t count) {
  static void_type_t *void_type{::new void_type_t()};
  std::cout << "void_type_t new.\n";
  return void_type;
}
void void_type_t::operator delete(void *ptr) {
  std::cout << "void_type_t delete.\n";
}
std::unique_ptr<build_type> void_type_t::get() {
  return std::unique_ptr<void_type_t>{new void_type_t()};
}
llvm::Type *void_type_t::get_llvm_type() const { return Builder->getVoidTy(); }
std::string const void_type_t::decorated_name() const { return "_void"s; }

void *bool_type_t::operator new(std::size_t count) {
  static bool_type_t *bool_type{::new bool_type_t()};
  return bool_type;
}
void bool_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> bool_type_t::get() {
  return std::unique_ptr<bool_type_t>{new bool_type_t()};
}
llvm::Type *bool_type_t::get_llvm_type() const { return Builder->getInt1Ty(); }
std::string const bool_type_t::decorated_name() const { return "_bool"s; }

void *size_type_t::operator new(std::size_t count) {
  static size_type_t *size_type{::new size_type_t()};
  return size_type;
}
void size_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> size_type_t::get() {
  return std::unique_ptr<size_type_t>{new size_type_t()};
}
llvm::Type *size_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const size_type_t::decorated_name() const { return "_size"s; }

void *i8_type_t::operator new(std::size_t count) {
  static i8_type_t *i8_type{::new i8_type_t()};
  return i8_type;
}
void i8_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> i8_type_t::get() {
  return std::unique_ptr<i8_type_t>{new i8_type_t()};
}
llvm::Type *i8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const i8_type_t::decorated_name() const { return "_i8"s; }

void *i16_type_t::operator new(std::size_t count) {
  static i16_type_t *i16_type{::new i16_type_t()};
  return i16_type;
}
void i16_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> i16_type_t::get() {
  return std::unique_ptr<i16_type_t>{new i16_type_t()};
}
llvm::Type *i16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const i16_type_t::decorated_name() const { return "_i16"s; }

void *i32_type_t::operator new(std::size_t count) {
  static i32_type_t *i32_type{::new i32_type_t()};
  return i32_type;
}
void i32_type_t::operator delete(void *ptr) {}
llvm::Type *i32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const i32_type_t::decorated_name() const { return "_i32"s; }

void *i64_type_t::operator new(std::size_t count) {
  static i64_type_t *i64_type{::new i64_type_t()};
  return i64_type;
}
void i64_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> i64_type_t::get() {
  return std::unique_ptr<i64_type_t>{new i64_type_t()};
}
llvm::Type *i64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const i64_type_t::decorated_name() const { return "_i64"s; }

void *i128_type_t::operator new(std::size_t count) {
  static i128_type_t *i128_type{::new i128_type_t()};
  return i128_type;
}
void i128_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> i128_type_t::get() {
  return std::unique_ptr<i128_type_t>{new i128_type_t()};
}
llvm::Type *i128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string const i128_type_t::decorated_name() const { return "_i128"s; }

void *u8_type_t::operator new(std::size_t count) {
  static u8_type_t *u8_type{::new u8_type_t()};
  return u8_type;
}
void u8_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> u8_type_t::get() {
  return std::unique_ptr<u8_type_t>{new u8_type_t()};
}
llvm::Type *u8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const u8_type_t::decorated_name() const { return "_u8"s; }

void *u16_type_t::operator new(std::size_t count) {
  static u16_type_t *u16_type{::new u16_type_t()};
  return u16_type;
}
void u16_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> u16_type_t::get() {
  return std::unique_ptr<u16_type_t>{new u16_type_t()};
}
llvm::Type *u16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const u16_type_t::decorated_name() const { return "_u16"s; }

void *u32_type_t::operator new(std::size_t count) {
  static u32_type_t *u32_type{::new u32_type_t()};
  return u32_type;
}
void u32_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> u32_type_t::get() {
  return std::unique_ptr<u32_type_t>{new u32_type_t()};
}
llvm::Type *u32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const u32_type_t::decorated_name() const { return "_u32"s; }

void *u64_type_t::operator new(std::size_t count) {
  static u64_type_t *u64_type{::new u64_type_t()};
  return u64_type;
}
void u64_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> u64_type_t::get() {
  return std::unique_ptr<u64_type_t>{new u64_type_t()};
}
llvm::Type *u64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const u64_type_t::decorated_name() const { return "_u64"s; }

void *u128_type_t::operator new(std::size_t count) {
  static u128_type_t *u128_type{::new u128_type_t()};
  return u128_type;
}
void u128_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> u128_type_t::get() {
  return std::unique_ptr<u128_type_t>{new u128_type_t()};
}
llvm::Type *u128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string const u128_type_t::decorated_name() const { return "_u128"s; }

void *f16_type_t::operator new(std::size_t count) {
  static f16_type_t *f16_type{::new f16_type_t()};
  return f16_type;
}
void f16_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> f16_type_t::get() {
  return std::unique_ptr<f16_type_t>{new f16_type_t()};
}
llvm::Type *f16_type_t::get_llvm_type() const { return Builder->getHalfTy(); }
std::string const f16_type_t::decorated_name() const { return "_f16"s; }

void *f32_type_t::operator new(std::size_t count) {
  static f32_type_t *i32_type{::new f32_type_t()};
  return i32_type;
}
void f32_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> f32_type_t::get() {
  return std::unique_ptr<f32_type_t>{new f32_type_t()};
}
llvm::Type *f32_type_t::get_llvm_type() const { return Builder->getFloatTy(); }
std::string const f32_type_t::decorated_name() const { return "_f32"s; }

void *f64_type_t::operator new(std::size_t count) {
  static f64_type_t *i32_type{::new f64_type_t()};
  return i32_type;
}
void f64_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> f64_type_t::get() {
  return std::unique_ptr<f64_type_t>{new f64_type_t()};
}
llvm::Type *f64_type_t::get_llvm_type() const { return Builder->getDoubleTy(); }
std::string const f64_type_t::decorated_name() const { return "_f64"s; }

void *f128_type_t::operator new(std::size_t count) {
  static f128_type_t *i32_type{::new f128_type_t()};
  return i32_type;
}
void f128_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> f128_type_t::get() {
  return std::unique_ptr<f128_type_t>{new f128_type_t()};
}
llvm::Type *f128_type_t::get_llvm_type() const {
  return llvm::Type::getFP128Ty(*TheContext);
}
std::string const f128_type_t::decorated_name() const { return "_f128"s; }

void *c8_type_t::operator new(std::size_t count) {
  static c8_type_t *i32_type{::new c8_type_t()};
  return i32_type;
}
void c8_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> c8_type_t::get() {
  return std::unique_ptr<c8_type_t>{new c8_type_t()};
}
llvm::Type *c8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const c8_type_t::decorated_name() const { return "_c8"s; }

void *c16_type_t::operator new(std::size_t count) {
  static c16_type_t *i32_type{::new c16_type_t()};
  return i32_type;
}
void c16_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> c16_type_t::get() {
  return std::unique_ptr<c16_type_t>{new c16_type_t()};
}
llvm::Type *c16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const c16_type_t::decorated_name() const { return "_c16"s; }

void *c32_type_t::operator new(std::size_t count) {
  static c32_type_t *i32_type{::new c32_type_t()};
  return i32_type;
}
void c32_type_t::operator delete(void *ptr) {}
std::unique_ptr<build_type> c32_type_t::get() {
  return std::unique_ptr<c32_type_t>{new c32_type_t()};
}
llvm::Type *c32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const c32_type_t::decorated_name() const { return "_c32"s; }

llvm::Type *arr_type::get_llvm_type() const {
  return llvm::ArrayType::get(m_element_type->get_llvm_type(), m_size);
}
// std::unique_ptr<build_type> arr_type::get() {}
std::string const arr_type::decorated_name() const {
  return "_array"s + m_element_type->decorated_name() + std::to_string(m_size);
}
void arr_type::set_const() { std::cerr << "con't fix array with const.\n"; }
void arr_type::set_volatile() {
  std::cerr << "con't fix array with volatile.\n";
}

llvm::Type *multi_array_type::get_llvm_type() const {
  return llvm::ArrayType::get(m_element_type->get_llvm_type(),
                              std::accumulate(m_size.begin(), m_size.end(), 1,
                                              std::multiplies<std::size_t>{}));
}
std::string const multi_array_type::decorated_name() const {
  std::string str{"_mularray"s + m_element_type->decorated_name()};
  for (auto s : m_size) {
    str += "_"s;
    str += std::to_string(s);
  }
  return str;
}
void multi_array_type::set_const() {
  std::cerr << "array type can't set const.\n";
}
void multi_array_type::set_volatile() {
  std::cerr << "array type can't set volatile.\n";
}

llvm::Type *pointer_type::get_llvm_type() const {
  return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
}
std::string const pointer_type::decorated_name() const {
  return "_ptr"s + m_element_type->decorated_name();
}
void pointer_type::set_const() {
  if (is_const()) {
    std::cerr << "redef const.\n";
  } else {
    m_is_const = true;
  }
}
void pointer_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}

void reference_type::set_const() {
  std::cerr << "reference type can't set const.\n";
}
void reference_type::set_volatile() {
  std::cerr << "reference type can't set volatile.\n";
}
llvm::Type *reference_type::get_llvm_type() const {
  return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
}
std::string const reference_type::decorated_name() const {
  return "_ref"s + m_element_type->decorated_name();
}

std::unique_ptr<build_type> type_build(base_build *const father,
                                       type_ast const *const ast) {
  return ast->build(father);
}
std::unique_ptr<reference_type>
build_reference_type(base_build *const father, reference_ast const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  return std::make_unique<reference_type>(std::move(element));
}

std::unique_ptr<build_type> build_const_type(base_build *const father,
                                             const_type const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  element->set_const();
  return element;
}
std::unique_ptr<build_type>
build_volatile_type(base_build *const father, volatile_type const *const ast) {
  auto element = type_build(father, ast->get_element_type());
  element->set_volatile();
  return element;
}
} // namespace pache