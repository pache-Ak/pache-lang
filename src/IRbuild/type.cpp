#include "type.h"
#include "../ast/type.h"
#include "ast/multi_array_type.h"
#include "expression.h"
#include "build.h"
#include "function_type.h"
#include "scope.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <iostream>
#include <memory>
#include <numeric>
#include <string_view>
#include <vector>
#include "class_type.h"
#include <utility>

using namespace std::literals;

namespace pache {
type::type_ID type::get_type_ID() const { return m_ID; }
bool type::is_void() const { return get_type_ID() == type_ID::VOID; }
bool type::is_bool() const { return get_type_ID() == type_ID::BOOL; }
bool type::is_byte() const { return get_type_ID() == type_ID::BYTE; }

bool type::is_signed() const {
  return (static_cast<std::underlying_type_t<type_ID>>(get_type_ID()) &
         0xf8) == 0x08;
}
bool type::is_unsigned() const {
  return (static_cast<std::underlying_type_t<type_ID>>(get_type_ID()) &
         0xf8) == 0x00;
}

std::string_view type::decorated_name() const {
  switch (get_type_ID()) {
  case type_ID::U8:
    return u8_type_t::decorated_name();
  case type_ID::U16:
    return u16_type_t::decorated_name();
  case type_ID::U32:
    return u32_type_t::decorated_name();
  case type_ID::U64:
    return u64_type_t::decorated_name();
//  case type_ID::U128:
//    return u128_type_t::decorated_name();
//  case type_ID::U256:
//    return u256_type_t::decorated_name();
  case type_ID::I8:
    return i8_type_t::decorated_name();
  case type_ID::I16:
    return i16_type_t::decorated_name();
  case type_ID::I32:
    return i32_type_t::decorated_name();
  case type_ID::I64:
    return i64_type_t::decorated_name();
//  case type_ID::I128:
//    return i128type_t::decorated_name();
//  case type_ID::I256:
//    return i256_type_t::decorated_name();
  case type_ID::F16:
    return f16_type_t::decorated_name();
  case type_ID::F32:
    return f32_type_t::decorated_name();
  case type_ID::F64:
    return f64_type_t::decorated_name();
  case type_ID::F128:
    return f128_type_t::decorated_name();
//  case type_ID::D32:
//    return d32type_t::decorated_name();
//  case type_ID::D64:
//    return d64type_t::decorated_name();
//  case type_ID::D128:
//    return d128_type_t::decorated_name();
  case type_ID::C8:
    return c8_type_t::decorated_name();
  case type_ID::C16:
    return c16_type_t::decorated_name();
  case type_ID::C32:
    return c32_type_t::decorated_name();
  case type_ID::VOID:
    return void_type_t::decorated_name();
  case type_ID::BOOL:
    return bool_type_t::decorated_name();
  case type_ID::BYTE:
    return byte_t::decorated_name();
  case type_ID::SIZE:
    return size_type_t::decorated_name();
  case type_ID::ARRAY:
    return static_cast<arr_type const*>(this)->arr_type::decorated_name();
  case type_ID::STRUCT:
    return static_cast<class_type const*>(this)->class_type::decorated_name();
  case type_ID::POINTER:
    return static_cast<pointer_type const*>(this)->pointer_type::decorated_name();
  case type_ID::REFERENCE:
    return static_cast<reference_type const*>(this)->reference_type::decorated_name();
  case type_ID::FUNCTION:
    return static_cast<function_type const *>(this)->function_type::decorated_name();
  default:
    return "error type id.\n";
  }
}
bool type::is_floating_point() const {
  return static_cast<std::underlying_type_t<type_ID>>(get_type_ID()) &
         0xe0 == 0x10;
}
type::type_ID operator|(type::type_ID lhs, std::underlying_type_t<type::type_ID>  rhs) {
  return static_cast<type::type_ID>(static_cast<std::underlying_type_t<type::type_ID>>(lhs) | rhs);
}
void type::set_mutable() {
  if (get_type_ID() == type_ID::ARRAY) {
    std::cerr << "con't fix array with mut.\n";
  } else if (get_type_ID() == type_ID::FUNCTION) {
    std::cerr << "can't fix function with mut.";
  } else if (get_type_ID() == type_ID::REFERENCE) {
    std::cerr << "reference type can't set mut.\n";
  } else if (!is_const()) {
    std::cerr << "redef mutable.\n";
  } else {
    m_ID = m_ID | 0x80;
  }
}
void type::set_volatile() {
  if (get_type_ID() == type_ID::ARRAY) {
    std::cerr << "con't fix array with volatile.\n";
  } else if (get_type_ID() == type_ID::FUNCTION) {
    std::cerr << "can't fix function with volatile.";
  } else if (get_type_ID() == type_ID::REFERENCE) {
    std::cerr << "reference type can't set volatile.\n";
  } else if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_ID = m_ID | 0x40;
  }
}
bool type::is_volatile() const {
  if (get_type_ID() == type_ID::ARRAY) {
    std::cerr << "con't fix array with volatile.\n";
  } else if (get_type_ID() == type_ID::FUNCTION) {
    std::cerr << "can't fix function with volatile.\n";
  } else if (get_type_ID() == type_ID::REFERENCE) {
    std::cerr << "can't fix reference with volatile.\n";
  } else {
    return static_cast<std::underlying_type_t<type_ID>>(get_type_ID()) & 0x40 == 0x40;
  }

  return false;
}
bool type::is_const() const {
  if (get_type_ID() == type_ID::ARRAY) {
    std::cerr << "con't fix array with mut.\n";
  } else if (get_type_ID() == type_ID::FUNCTION) {
    std::cerr << "can't fix function with mut.\n";
  } else if (get_type_ID() == type_ID::REFERENCE) {
    std::cerr << "can't fix reference with mut.\n";
  } else {
    return static_cast<std::underlying_type_t<type_ID>>(get_type_ID()) & 0x80 == 0x80;
  }

  return false;
}
bool type::is_arithmetic() const {
  return is_integral() || is_floating_point();
}
bool type::is_integral() const { return is_signed() || is_unsigned(); }
bool type::is_reference() const { return get_type_ID() == type_ID::REFERENCE; }
bool type::is_array() const { return get_type_ID() == type_ID::ARRAY; }
bool type::is_struct() const { return get_type_ID() == type_ID::STRUCT; }
bool type::is_function() const { return get_type_ID() == type_ID::FUNCTION; }
bool type::is_pointer() const { return get_type_ID() == type_ID::POINTER; }

llvm::Type *get_llvm_type(std::unique_ptr<build_type> const &type) {
  return type->get_llvm_type();
}

build_type::~build_type(){}
inline namespace integral {
  integral_type::~integral_type(){}
  signed_type::~signed_type(){}
  unsigned_type::~unsigned_type(){}
}


llvm::Type *void_type_t::get_llvm_type() const { return Builder->getVoidTy(); }
constexpr std::string_view void_type_t::decorated_name() {
  return "_T4void"sv;
}
std::unique_ptr<build_type> void_type_t::clone() const {
  return std::make_unique<void_type_t>();
}

llvm::Type *bool_type_t::get_llvm_type() const { return Builder->getInt1Ty(); }
constexpr std::string_view bool_type_t::decorated_name() {
  return "_T4bool"sv;
}
std::unique_ptr<build_type> bool_type_t::clone() const {
  return std::make_unique<bool_type_t>();
}

llvm::Type *size_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
constexpr std::string_view size_type_t::decorated_name() {
  return "_T4size"sv;
}
std::unique_ptr<build_type> size_type_t::clone() const {
  return std::make_unique<size_type_t>();
}

llvm::Type *byte_t::get_llvm_type() const { return Builder->getInt8Ty(); }
constexpr std::string_view byte_t::decorated_name() { return "T4_byte"sv; }
std::unique_ptr<build_type> byte_t::clone() const {
  return std::make_unique<byte_t>();
}

llvm::Type *i8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string_view i8_type_t::decorated_name() { return "_T2i8"sv; }
std::unique_ptr<build_type> i8_type_t::clone() const {
  return std::make_unique<i8_type_t>();
}

llvm::Type *i16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string_view i16_type_t::decorated_name() { return "_T2i16"sv; }
std::unique_ptr<build_type> i16_type_t::clone() const {
  return std::make_unique<i16_type_t>();
}

llvm::Type *i32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string_view i32_type_t::decorated_name() { return "_T2i32"sv; }
std::unique_ptr<build_type> i32_type_t::clone() const {
  return std::make_unique<i32_type_t>();
}

llvm::Type *i64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string_view i64_type_t::decorated_name() { return "_T2i64"sv; }
std::unique_ptr<build_type> i64_type_t::clone() const {
  return std::make_unique<i64_type_t>();
}
/* 
llvm::Type *i128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string_view i128_type_t::decorated_name() { return "_T3i128"sv; }
std::unique_ptr<build_type> i128_type_t::clone() const {
  return std::make_unique<i128_type_t>();
}
 */
llvm::Type *u8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string_view u8_type_t::decorated_name() { return "_u8"sv; }
std::unique_ptr<build_type> u8_type_t::clone() const {
  return std::make_unique<u8_type_t>();
}

llvm::Type *u16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string_view u16_type_t::decorated_name() { return "_u16"sv; }
std::unique_ptr<build_type> u16_type_t::clone() const {
  return std::make_unique<u16_type_t>();
}

llvm::Type *u32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string_view u32_type_t::decorated_name() { return "_u32"sv; }
std::unique_ptr<build_type> u32_type_t::clone() const {
  return std::make_unique<u32_type_t>();
}

llvm::Type *u64_type_t::get_llvm_type() const { return Builder->getInt64Ty(); }
std::string_view u64_type_t::decorated_name() { return "_u64"sv; }
std::unique_ptr<build_type> u64_type_t::clone() const {
  return std::make_unique<u64_type_t>();
}
/* 
llvm::Type *u128_type_t::get_llvm_type() const {
  return Builder->getInt128Ty();
}
std::string_view u128_type_t::decorated_name() { return "_u128"sv; }
std::unique_ptr<build_type> u128_type_t::clone() const {
  return std::make_unique<u128_type_t>();
} */

llvm::Type *f16_type_t::get_llvm_type() const { return Builder->getHalfTy(); }
std::string_view f16_type_t::decorated_name() { return "_f16"sv; }
std::unique_ptr<build_type> f16_type_t::clone() const {
  return std::make_unique<f16_type_t>();
}

llvm::Type *f32_type_t::get_llvm_type() const { return Builder->getFloatTy(); }
std::string_view f32_type_t::decorated_name() { return "_f32"sv; }
std::unique_ptr<build_type> f32_type_t::clone() const {
  return std::make_unique<f32_type_t>();
}

llvm::Type *f64_type_t::get_llvm_type() const { return Builder->getDoubleTy(); }
std::string_view f64_type_t::decorated_name() { return "_f64"sv; }
std::unique_ptr<build_type> f64_type_t::clone() const {
  return std::make_unique<f64_type_t>();
}

llvm::Type *f128_type_t::get_llvm_type() const {
  return llvm::Type::getFP128Ty(*TheContext);
}
std::string_view f128_type_t::decorated_name() { return "_f128"sv; }
std::unique_ptr<build_type> f128_type_t::clone() const {
  return std::make_unique<f128_type_t>();
}


// llvm::Type *d32_type_t::get_llvm_type() const {}
// std::string_view d32_type_t::decorated_name() { return "_d32"sv; }
// std::unique_ptr<build_type> d32_type_t::clone() const {
//   return std::make_unique<d32_type_t>();
// }

// llvm::Type *d64_type_t::get_llvm_type() const {  }
// std::string_view d64_type_t::decorated_name() { return "_d64"sv; }
// std::unique_ptr<build_type> d64_type_t::clone() const {
//   return std::make_unique<d64_type_t>();
// }

// llvm::Type *d128_type_t::get_llvm_type() const {}
// std::string_view d128_type_t::decorated_name() { return "_d128"sv; }
// std::unique_ptr<build_type> d128_type_t::clone() const {
//   return std::make_unique<d128_type_t>();
// }


llvm::Type *c8_type_t::get_llvm_type() const { return Builder->getInt8Ty(); }
std::string_view c8_type_t::decorated_name() { return "_c8"sv; }
std::unique_ptr<build_type> c8_type_t::clone() const {
  return std::make_unique<c8_type_t>();
}

llvm::Type *c16_type_t::get_llvm_type() const { return Builder->getInt16Ty(); }
std::string_view c16_type_t::decorated_name() { return "_c16"sv; }
std::unique_ptr<build_type> c16_type_t::clone() const {
  return std::make_unique<c16_type_t>();
}

llvm::Type *c32_type_t::get_llvm_type() const { return Builder->getInt32Ty(); }
std::string_view c32_type_t::decorated_name() { return "_c32"sv; }
std::unique_ptr<build_type> c32_type_t::clone() const {
  return std::make_unique<c32_type_t>();
}

llvm::Type *arr_type::get_llvm_type() const {
  return llvm::ArrayType::get(
    m_element_type->get_llvm_type(),
    std::accumulate(m_size.begin(), m_size.end(), 1,
                    std::multiplies<std::size_t>{}));
}
std::string_view arr_type::decorated_name() const {
   std::string str{"_mularray"s + std::string{m_element_type->decorated_name()} };
   for (auto s : m_size) {
     str += "_"s;
     str += std::to_string(s);
   }
   return str;
}
std::unique_ptr<build_type> arr_type::clone() const {
  return std::unique_ptr<arr_type>(
      new arr_type(m_element_type->clone(), m_size));
}

// llvm::Type *multi_array_type::get_llvm_type() const {
//   return llvm::ArrayType::get(m_element_type->get_llvm_type(),
//                               std::accumulate(m_size.begin(), m_size.end(), 1,
//                                               std::multiplies<std::size_t>{}));
// }
// std::string_view multi_array_type::decorated_name() {
//   std::string str{"_mularray"s + m_element_type->decorated_name()};
//   for (auto s : m_size) {
//     str += "_"s;
//     str += std::to_string(s);
//   }
//   return str;
// }
// void multi_array_type::set_mutable() {
//   std::cerr << "array type can't set mut.\n";
// }
// void multi_array_type::set_volatile() {
//   std::cerr << "array type can't set volatile.\n";
// }

// std::unique_ptr<build_type> multi_array_type::clone() const {
//   return std::make_unique<multi_array_type>(m_element_type->clone(), m_size);
// }

pointer_type::pointer_type(std::unique_ptr<build_type> &&element_type)
    : m_element_type(std::move(element_type)) {}
llvm::Type *pointer_type::get_llvm_type() const {
  return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
}
std::string_view pointer_type::decorated_name() const {
  return "_ptr"s + std::string{m_element_type->decorated_name()};
}

std::unique_ptr<build_type> pointer_type::clone() const {
  return std::make_unique<pointer_type>(m_element_type->clone());
}

reference_type::reference_type(std::unique_ptr<build_type> &&element_type)
    : m_element_type(std::move(element_type)) {}
llvm::Type *reference_type::get_llvm_type() const {
  return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
}
std::string_view reference_type::decorated_name() const {
  return "_ref"s + std::string{m_element_type->decorated_name()};
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
  return std::make_unique<reference_type>(type_build(father, ast.get_element_type()));
}

std::unique_ptr<build_type>
build_mut_type(base_build &father, mut_ast const &ast) {
  auto element = type_build(father, ast.get_element_type());
  element->set_mutable();
  return element;
}
std::unique_ptr<build_type>
build_volatile_type(base_build &father, volatile_ast const &ast) {
  auto element = type_build(father, ast.get_element_type());
  element->set_volatile();
  return element;
}

std::unique_ptr<build_type>
build_pointer_type(base_build &father, pointer_ast const &ast) {
  return std::make_unique<pointer_type>(type_build(father, ast.get_element_type()));
}

// std::unique_ptr<arr_type>
// build_arr_type(base_build &father, arr_ast const &ast) {
//   return std::make_unique<arr_type>(type_build(father, *ast.get_element_type()), ast.get_size());
// }

 std::unique_ptr<arr_type>
 build_multi_array_type(base_build &father, multi_array_ast const &ast) {
  std::vector<std::size_t> size;
  for (auto const &exp : ast.get_size()) {
    auto val = build_expression(father, *exp);
    if (llvm::ConstantInt *constantInt =
            llvm::dyn_cast<llvm::ConstantInt>(val->get_value());
        constantInt != nullptr) {
      llvm::APInt intValue = constantInt->getValue();
      size.emplace_back(static_cast<std::size_t>(intValue.getZExtValue()));
    } else {
      std::cerr << "Value is not an integer constant\n";
      exp->print();
    }
  }
   return std::make_unique<arr_type>(type_build(father, *ast.get_element_type()), std::move(size));
 }

std::unique_ptr<build_type>
build_named_type(base_build &father, named_type_ast const &ast) {
  std::unique_ptr<build_scope> p{ast.get_father_scope().build(father)};
  if (p != nullptr) {
    if (auto it = p->find_type(ast.get_name()); it != nullptr) {
      return it->clone();
    }
  }

  return nullptr;
}
bool operator==(build_type const &lhs, build_type const &rhs) {
  return lhs.decorated_name() == rhs.decorated_name();
}
bool operator!=(build_type const &lhs, build_type const &rhs) {
  return lhs.decorated_name() != rhs.decorated_name();
}

} // namespace pache