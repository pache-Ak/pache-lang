#include "type.h"
#include "../ast/type.h"
#include "build.h"
#include "llvm/IR/Type.h"
#include <iostream>
#include <memory>
#include <string>
#include <numeric>

using namespace std::literals;

namespace pache {
llvm::Type *get_llvm_type(std::unique_ptr<build_type> const &type) {
  return type->get_llvm_type();
}

build_type::~build_type(){}
bool build_type::is_integral() const { return false; }
bool build_type::is_signed() const { return false; }
bool build_type::is_unsigned() const { return false; }
bool build_type::is_floating_point() const { return false; }
bool build_type::is_decimal_floating_pointer() const { return false; }
std::string const build_type::decorated_name() const { return ""s; }
bool build_type::is_const() const { return m_is_const; }
bool build_type::is_volatile() const { return m_is_volatile; }
bool build_type::is_character() const { return false; }

void primary_type::set_mutable() {
  if (!is_const()) {
    std::cerr << "redef mutable.\n";
  } else {
    m_is_const = false;
  }
}
void primary_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}

llvm::Type *void_type_t::get_llvm_type() const { return Builder->getVoidTy(); }
std::string const void_type_t::decorated_name() const { return "_void"s; }
std::unique_ptr<build_type> void_type_t::clone() const {
  return std::unique_ptr<void_type_t>(new void_type_t(*this));
}

llvm::Type *bool_type_t::get_llvm_type() const { return Builder->getInt1Ty(); }
std::string const bool_type_t::decorated_name() const { return "_bool"s; }
std::unique_ptr<build_type> bool_type_t::clone() const {
  return std::unique_ptr<bool_type_t>(new bool_type_t(*this));
}

llvm::Type *size_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const size_type_t::decorated_name() const { return "_size"s; }
std::unique_ptr<build_type> size_type_t::clone() const {
  return std::unique_ptr<size_type_t>(new size_type_t(*this));
}

llvm::Type *i8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const i8_type_t::decorated_name() const { return "_i8"s; }
std::unique_ptr<build_type> i8_type_t::clone() const {
  return std::unique_ptr<i8_type_t>(new i8_type_t(*this));
}

llvm::Type *i16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const i16_type_t::decorated_name() const { return "_i16"s; }
std::unique_ptr<build_type> i16_type_t::clone() const {
  return std::unique_ptr<i16_type_t>(new i16_type_t(*this));
}

llvm::Type *i32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const i32_type_t::decorated_name() const { return "_i32"s; }
std::unique_ptr<build_type> i32_type_t::clone() const {
  return std::unique_ptr<i32_type_t>(new i32_type_t(*this));
}

llvm::Type *i64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const i64_type_t::decorated_name() const { return "_i64"s; }
std::unique_ptr<build_type> i64_type_t::clone() const {
  return std::unique_ptr<i64_type_t>(new i64_type_t(*this));
}
/* 
llvm::Type *i128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string const i128_type_t::decorated_name() const { return "_i128"s; }
std::unique_ptr<build_type> i128_type_t::clone() const {
  return std::unique_ptr<i128_type_t>(new i128_type_t(*this));
}
 */
llvm::Type *u8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const u8_type_t::decorated_name() const { return "_u8"s; }
std::unique_ptr<build_type> u8_type_t::clone() const {
  return std::unique_ptr<u8_type_t>(new u8_type_t(*this));
}

llvm::Type *u16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const u16_type_t::decorated_name() const { return "_u16"s; }
std::unique_ptr<build_type> u16_type_t::clone() const {
  return std::unique_ptr<u16_type_t>(new u16_type_t(*this));
}

llvm::Type *u32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const u32_type_t::decorated_name() const { return "_u32"s; }
std::unique_ptr<build_type> u32_type_t::clone() const {
  return std::unique_ptr<u32_type_t>(new u32_type_t(*this));
}

llvm::Type *u64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string const u64_type_t::decorated_name() const { return "_u64"s; }
std::unique_ptr<build_type> u64_type_t::clone() const {
  return std::unique_ptr<u64_type_t>(new u64_type_t(*this));
}
/* 
llvm::Type *u128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string const u128_type_t::decorated_name() const { return "_u128"s; }
std::unique_ptr<build_type> u128_type_t::clone() const {
  return std::unique_ptr<u128_type_t>(new u128_type_t(*this));
} */

bool floating_pointer_type::is_floating_point() const {
  return true;
}

llvm::Type *f16_type_t::get_llvm_type() const { return Builder->getHalfTy(); }
std::string const f16_type_t::decorated_name() const { return "_f16"s; }
std::unique_ptr<build_type> f16_type_t::clone() const {
  return std::unique_ptr<f16_type_t>(new f16_type_t(*this));
}

llvm::Type *f32_type_t::get_llvm_type() const { return Builder->getFloatTy(); }
std::string const f32_type_t::decorated_name() const { return "_f32"s; }
std::unique_ptr<build_type> f32_type_t::clone() const {
  return std::unique_ptr<f32_type_t>(new f32_type_t(*this));
}

llvm::Type *f64_type_t::get_llvm_type() const { return Builder->getDoubleTy(); }
std::string const f64_type_t::decorated_name() const { return "_f64"s; }
std::unique_ptr<build_type> f64_type_t::clone() const {
  return std::unique_ptr<f64_type_t>(new f64_type_t(*this));
}

llvm::Type *f128_type_t::get_llvm_type() const {
  return llvm::Type::getFP128Ty(*TheContext);
}
std::string const f128_type_t::decorated_name() const { return "_f128"s; }
std::unique_ptr<build_type> f128_type_t::clone() const {
  return std::unique_ptr<f128_type_t>(new f128_type_t(*this));
}

bool decimal_floating_pointer_type::is_decimal_floating_pointer() const {
  return true;
}

llvm::Type *d32_type_t::get_llvm_type() const { return Builder->getFloatTy();}
std::string const d32_type_t::decorated_name() const { return "_d32"s; }
std::unique_ptr<build_type> d32_type_t::clone() const {
  return std::unique_ptr<d32_type_t>(new d32_type_t{*this});
}

llvm::Type *d64_type_t::get_llvm_type() const { return Builder->getDoubleTy(); }
std::string const d64_type_t::decorated_name() const { return "_d64"s; }
std::unique_ptr<build_type> d64_type_t::clone() const {
  return std::unique_ptr<d64_type_t>(new d64_type_t{*this});
}

llvm::Type *d128_type_t::get_llvm_type() const {}
std::string const d128_type_t::decorated_name() const { return "_d128"s; }
std::unique_ptr<build_type> d128_type_t::clone() const {
  return std::unique_ptr<d128_type_t>(new d128_type_t{*this});
}

bool character_type::is_character() const {
  return true;
}

llvm::Type *c8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string const c8_type_t::decorated_name() const { return "_c8"s; }
std::unique_ptr<build_type> c8_type_t::clone() const {
  return std::unique_ptr<c8_type_t>(new c8_type_t(*this));
}

llvm::Type *c16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string const c16_type_t::decorated_name() const { return "_c16"s; }
std::unique_ptr<build_type> c16_type_t::clone() const {
  return std::unique_ptr<c16_type_t>(new c16_type_t(*this));
}

llvm::Type *c32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string const c32_type_t::decorated_name() const { return "_c32"s; }
std::unique_ptr<build_type> c32_type_t::clone() const {
  return std::unique_ptr<c32_type_t>(new c32_type_t(*this));
}

llvm::Type *arr_type::get_llvm_type() const {
  return llvm::ArrayType::get(m_element_type->get_llvm_type(), m_size);
}
// std::unique_ptr<build_type> arr_type::get() {}
std::string const arr_type::decorated_name() const {
  return "_array"s + m_element_type->decorated_name() + std::to_string(m_size);
}
void arr_type::set_mutable() { std::cerr << "con't fix array with mut.\n"; }
void arr_type::set_volatile() {
  std::cerr << "con't fix array with volatile.\n";
}
std::unique_ptr<build_type> arr_type::clone() const {
  return std::unique_ptr<arr_type>(
      new arr_type(m_element_type->clone(), m_size));
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
void multi_array_type::set_mutable() {
  std::cerr << "array type can't set mut.\n";
}
void multi_array_type::set_volatile() {
  std::cerr << "array type can't set volatile.\n";
}

std::unique_ptr<build_type> multi_array_type::clone() const {
  return std::make_unique<multi_array_type>(m_element_type->clone(), m_size);
}

pointer_type::pointer_type(std::unique_ptr<build_type> &&element_type)
    : m_element_type(std::move(element_type)) {}
llvm::Type *pointer_type::get_llvm_type() const {
  return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
}
std::string const pointer_type::decorated_name() const {
  return "_ptr"s + m_element_type->decorated_name();
}
void pointer_type::set_mutable() {
  if (!is_const()) {
    std::cerr << "redef mut.\n";
  } else {
    m_is_const = false;
  }
}
void pointer_type::set_volatile() {
  if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  }
}
std::unique_ptr<build_type> pointer_type::clone() const {
  return std::make_unique<pointer_type>(m_element_type->clone());
}

reference_type::reference_type(std::unique_ptr<build_type> &&element_type)
    : m_element_type(std::move(element_type)) {}
void reference_type::set_mutable() {
  std::cerr << "reference type can't set mut.\n";
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
std::unique_ptr<build_type> reference_type::clone() const {
  return std::make_unique<reference_type>(m_element_type->clone());
}

std::unique_ptr<build_type>
type_build(base_build &father, type_ast const &ast) {
  return ast.build(father);
}
std::unique_ptr<reference_type>
build_reference_type(base_build &father, reference_ast const &ast) {
  return std::make_unique<reference_type>(type_build(father, *ast.get_element_type()));
}

std::unique_ptr<build_type>
build_mut_type(base_build &father, mut_ast const &ast) {
  auto element = type_build(father, *ast.get_element_type());
  element->set_mutable();
  return element;
}
std::unique_ptr<build_type>
build_volatile_type(base_build &father, volatile_ast const &ast) {
  auto element = type_build(father, *ast.get_element_type());
  element->set_volatile();
  return element;
}

std::unique_ptr<build_type>
build_pointer_type(base_build &father, pointer_ast const &ast) {
  return std::make_unique<pointer_type>(type_build(father, *ast.get_element_type()));
}

std::unique_ptr<arr_type>
build_arr_type(base_build &father, arr_ast const &ast) {
  return std::make_unique<arr_type>(type_build(father, *ast.get_element_type()), ast.get_size());
}

std::unique_ptr<multi_array_type>
build_multi_array_type(base_build &father, multi_array_ast const &ast) {
  return std::make_unique<multi_array_type>(type_build(father, *ast.get_element_type()), ast.get_size());
}

std::unique_ptr<build_type>
build_named_type(base_build &father, named_ast const &ast) {
  return father.find_type(ast.get_name());
}
} // namespace pache