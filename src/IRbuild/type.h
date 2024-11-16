#ifndef IR_TYPE_H
#define IR_TYPE_H

#include <cstdint>
#include "build.h"
#include <cstddef>
#include <memory>
#include <string_view>
#include <vector>

namespace pache {
class type {
public:
  // 0x00 ~ 0x07 is unsigned
  // 0x08 ~ 0x0f is signed
  // 0x10 ~ 0x17 is binary float
  // 0x18 ~ 0x1b is deciamel float
  // 0x1c ~ 0x1f is character
  // 0x20 ~ 0x3f is other
  // 7th bit means volatile
  // 8th bit means mut
  enum class type_ID : std::uint8_t {
    U8 = 0b0000'0000, U16, U32, U64, U128, U256,
    I8 = 0b0000'1000, I16, I32, I64, I128, I256,
    F16 = 0b0001'0000, F32, F64, F128,
    D32 = 0b0001'1000, D64, D128,
    C8 = 0b0001'1100, C16, C32,
    VOID = 0b0010'0000, BOOL, BYTE, SIZE, 
    ARRAY, STRUCT, POINTER, REFERENCE, FUNCTION,
    NUL
  };

  type_ID get_type_ID() const;
  bool is_void() const;
  bool is_bool() const;
  bool is_byte() const;

  bool is_arithmetic() const;
  bool is_integral() const;
  bool is_signed() const;
  bool is_unsigned() const;
  bool is_floating_point() const;
  bool is_decimal_floating_pointer() const;
  bool is_character() const;
  std::string_view decorated_name() const;
  void set_mutable();
  void set_volatile();
  bool is_const() const;
  bool is_volatile() const;
  bool is_reference() const;
  bool is_array() const;
  bool is_struct() const;
  bool is_function() const;
  bool is_pointer() const;

protected:
  explicit type() : m_is_const(true), m_is_volatile(false) {}
  explicit type(type_ID id) : m_is_const(true), m_is_volatile(true), m_ID(id) {}
  ~type() noexcept = default;
  bool m_is_const;
  bool m_is_volatile;

protected:
  type_ID m_ID;

};

}

namespace llvm {
class Type;
}

namespace pache {
llvm::Type *get_llvm_type(std::unique_ptr<build_type> const &type);
template<class Iterator>
std::vector<llvm::Type*> get_llvm_type_arr(Iterator begin, Iterator end) {
  std::vector<llvm::Type*> llvm_type;
  std::transform(begin, end, std::back_inserter(llvm_type), get_llvm_type);
  return llvm_type;
}

class build_type : public type {
public:
  build_type() = default;
  virtual llvm::Type *get_llvm_type() const = 0;
  virtual ~build_type() = 0;

  virtual std::unique_ptr<build_type> clone() const = 0;

protected:
  build_type(type::type_ID id) : type(id) {}
  build_type(build_type const &other) = default;
  build_type(build_type &&other) = default;
  build_type &operator=(build_type const &other) = default;
  build_type &operator=(build_type &&other) = default;

private:
};

bool operator==(build_type const &lhs, build_type const &rhs);
bool operator!=(build_type const &lhs, build_type const &rhs);

class type_ast;
std::unique_ptr<build_type> type_build(base_build &father, type_ast const &ast);

inline namespace primary {
class primary_type : public build_type {
public:
  primary_type() = default;

protected:
  primary_type(type::type_ID id) : build_type(id) {}
  primary_type(primary_type const &other) = default;
  primary_type(primary_type &&other) = default;
  primary_type &operator=(primary_type const &other) = default;
  primary_type &operator=(primary_type &&other) = default;
};

class void_type_t final : public primary_type {
public:
  void_type_t() : primary_type(type::type_ID::VOID){}
  virtual llvm::Type *get_llvm_type() const override;
  static constexpr std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
  virtual ~void_type_t() =default;
};

class bool_type_t final : public primary_type {
public:
  bool_type_t() : primary_type(type::type_ID::BOOL){}
  virtual llvm::Type *get_llvm_type() const override;
  static constexpr std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class size_type_t final : public primary_type {
public:
  size_type_t() : primary_type(type::type_ID::SIZE){}
  virtual llvm::Type *get_llvm_type() const override;
  static constexpr std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class byte_t final : public primary_type {
public:
  using c_type = std::byte;
  byte_t() : primary_type(type::type_ID::BYTE){}
  virtual llvm::Type *get_llvm_type() const override;
  static constexpr std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

inline namespace integral {
class integral_type : public primary_type {
public:
  integral_type() = default;
  virtual ~integral_type() = 0;
  
protected:
  integral_type(type::type_ID id) : primary_type(id) {}
  integral_type(integral_type const &other) = default;
  integral_type &operator=(integral_type const &other) = default;
};

// name SICR mean signed integer conversion rank
enum class SICR : std::uint8_t {
  I8,
  I16,
  I32,
  I64,
  I128,
  NUL, //表示非法情况实现的占位符
};

class signed_type : public integral_type {
public:
  signed_type() = default;
  virtual ~signed_type() = 0;
  SICR get_conversion_rank() const {
    switch (m_ID) {
      case type::type_ID::I8:
        return SICR::I8;
      case type::type_ID::I16:
        return SICR::I16;
      case type::type_ID::I32:
        return SICR::I32;
      case type::type_ID::I64:
        return SICR::I64;
      case type::type_ID::I128:
        return SICR::I128;
      default:
        return SICR::NUL;
    }
  }
protected:
  signed_type(type::type_ID id) : integral_type(id) {}
  signed_type(signed_type const &other) = default;
  signed_type &operator=(signed_type const &other) = default;
};



// name SICR mean signed integer conversion rank
enum class UICR {
  U8,
  U16,
  U32,
  U64,
  U128,
  NUL, //表示非法情况实现的占位符
};
class unsigned_type : public integral_type {
public:
  virtual ~unsigned_type() = 0;
  unsigned_type() = default;
  UICR get_conversion_rank() const {
    switch (m_ID) {
      case type::type_ID::U8:
        return UICR::U8;
      case type::type_ID::U16:
        return UICR::U16;
      case type::type_ID::U32:
        return UICR::U32;
      case type::type_ID::U64:
        return UICR::U64;
      case type::type_ID::U128:
        return UICR::U128;
      default:
        return UICR::NUL;
    }
  }
protected:
  unsigned_type(type::type_ID id) : integral_type(id) {}
  unsigned_type(unsigned_type const &other) = default;
  unsigned_type &operator=(unsigned_type const &other) = default;
};

class i8_type_t final : public signed_type {
public:
  using c_type =std::int8_t;
  i8_type_t() : signed_type(type::type_ID::I8) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class i16_type_t final : public signed_type {
public:
  using c_type = std::int16_t;
  i16_type_t() : signed_type(type::type_ID::I16) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class i32_type_t final : public signed_type {
public:
  using c_type = std::int32_t;
  i32_type_t() : signed_type(type::type_ID::I32) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class i64_type_t final : public signed_type {
public:
  using c_type = std::int64_t;
  i64_type_t() : signed_type(type::type_ID::I64) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

// class i128_type_t final : public signed_type {
// public:
//   using c_type = std::int128_t;

//  i128_type_t() : signed_type(type::type_ID::I128) {}
//   virtual llvm::Type *get_llvm_type() const override;
//  static std::string_view decorated_name();
//   virtual std::unique_ptr<build_type> clone() const override;
// };

std::unique_ptr<signed_type>
signed_common_type(signed_type const &lhs, signed_type const &rhs) {
  static SICR type[5][5]{
    { SICR::I8,    SICR::I16,   SICR::I32,   SICR::I64,   SICR::I128,},
    { SICR::I16,   SICR::I16,   SICR::I32,   SICR::I64,   SICR::I128,},
    { SICR::I32,   SICR::I32,   SICR::I32,   SICR::I64,   SICR::I128,},
    { SICR::I64,   SICR::I64,   SICR::I64,   SICR::I64,   SICR::I128,},
    { SICR::I128,  SICR::I128,  SICR::I128,  SICR::I128,  SICR::I128,},
  };

  switch (type[static_cast<std::size_t>(lhs.get_conversion_rank())][static_cast<std::size_t>(rhs.get_conversion_rank())]) {
    case SICR::I8:
      return std::make_unique<i8_type_t>();
    case SICR::I16:
      return std::make_unique<i16_type_t>();
    case SICR::I32:
      return std::make_unique<i32_type_t>();
    case SICR::I64:
      return std::make_unique<i64_type_t>();
    //case SICR::I128:
   //   return std::make_unique<i1288_type_t>();
    default: ;
      return nullptr;
  }
}

class u8_type_t final : public unsigned_type {
public:
  using c_type = std::uint8_t;
  u8_type_t() : unsigned_type(type::type_ID::U8) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class u16_type_t final : public unsigned_type {
public:
  using c_type = std::uint16_t;  
  u16_type_t() : unsigned_type(type::type_ID::U16) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class u32_type_t final : public unsigned_type {
public:
  using c_type = std::uint32_t;
  u32_type_t() : unsigned_type(type::type_ID::U32) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class u64_type_t final : public unsigned_type {
public:
  using c_type = std::uint64_t;
  u64_type_t() : unsigned_type(type::type_ID::U64) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};


// class u128_type_t final : public primary_type {
// public:
//   using c_type = std::uint_128_t;
//  u128_type_t() : unsigned_type(type::type_ID::U128) {}
//   virtual llvm::Type *get_llvm_type() const override;
//  static std::string_view decorated_name();
//   virtual std::unique_ptr<build_type> clone() const override;
// };

std::unique_ptr<unsigned_type>
unsigned_common_type(unsigned_type const &lhs, unsigned_type const &rhs) {
  static UICR type[5][5]{
    { UICR::U8,    UICR::U16,   UICR::U32,   UICR::U64,   UICR::U128,},
    { UICR::U16,   UICR::U16,   UICR::U32,   UICR::U64,   UICR::U128,},
    { UICR::U32,   UICR::U32,   UICR::U32,   UICR::U64,   UICR::U128,},
    { UICR::U64,   UICR::U64,   UICR::U64,   UICR::U64,   UICR::U128,},
    { UICR::U128,  UICR::U128,  UICR::U128,  UICR::U128,  UICR::U128,},
  };

  switch (type[static_cast<std::size_t>(lhs.get_conversion_rank())][static_cast<std::size_t>(rhs.get_conversion_rank())]) {
    case UICR::U8:
      return std::make_unique<u8_type_t>();
    case UICR::U16:
      return std::make_unique<u16_type_t>();
    case UICR::U32:
      return std::make_unique<u32_type_t>();
    case UICR::U64:
      return std::make_unique<u64_type_t>();
    //case UICR::U128:
   //   return std::make_unique<u128_type_t>();
    default: ;
      return nullptr;
  }
}
} // namespace integral

inline namespace floating_pointer {

class floating_pointer_type : public primary_type {
public:
  floating_pointer_type() = default;
protected:
  floating_pointer_type(type::type_ID id) : primary_type(id) {}
  floating_pointer_type(floating_pointer_type const &other) = default;
  floating_pointer_type &
  operator=(floating_pointer_type const &other) = default;
};

class f16_type_t final : public floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  f16_type_t() : floating_pointer_type(type::type_ID::F16) {}
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

// a 16-bit bfloat type. iee754like
// class bfloat_type_t final : public floating_pointer_type {};

class f32_type_t final : public floating_pointer_type {
public:
  f32_type_t() : floating_pointer_type(type::type_ID::F32) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class f64_type_t final : public floating_pointer_type {
public:
  f64_type_t() : floating_pointer_type(type::type_ID::F64) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

// x86 long double
// class x86_fp80_type_t final : public floating_pointer_type {};

class f128_type_t final : public floating_pointer_type {
public:
  f128_type_t() : floating_pointer_type(type::type_ID::F128) {}
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

// powerpc long double
// class ppc_fp128_type_t final : public floating_pointer_type {};

inline namespace decimal_floating_pointer {
class decimal_floating_pointer_type : public floating_pointer_type {
public:

protected:
  decimal_floating_pointer_type(decimal_floating_pointer_type const &other) =
      default;
  decimal_floating_pointer_type &
  operator=(decimal_floating_pointer_type const &other) = default;
};

// class d32_type_t final : public decimal_floating_pointer_type {
// public:
//   virtual llvm::Type *get_llvm_type() const override;
//   virtual std::unique_ptr<build_type> clone() const override;
// };

// class d64_type_t final : public decimal_floating_pointer_type {
// public:
//   virtual llvm::Type *get_llvm_type() const override;
//   virtual std::unique_ptr<build_type> clone() const override;
// };

// class d128_type_t final : public decimal_floating_pointer_type {
// public:
//   virtual llvm::Type *get_llvm_type() const override;
//   virtual std::unique_ptr<build_type> clone() const override;
// };
} // namespace decimal_floating_pointer

} // namespace floating_pointer

inline namespace character {
class character_type : public primary_type {
public:
  character_type() =default;
protected:
  character_type(character_type const &other) = default;
  character_type &operator=(character_type const &other) = default;
};

class c8_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};

class c16_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};
class c32_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  static std::string_view decorated_name();
  virtual std::unique_ptr<build_type> clone() const override;
};
} // namespace character
} // namespace primary

class exp_ast;

class arr_type final : public build_type {
public:
  explicit arr_type(std::unique_ptr<build_type> &&element_type,
                            std::vector<std::size_t> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}  
  explicit arr_type(std::unique_ptr<build_type> &&element_type,
                            std::vector<std::size_t> const &size)
      : m_element_type(std::move(element_type)), m_size(size) {}   
  virtual llvm::Type *get_llvm_type() const override;
  std::string_view decorated_name() const;

  build_type const &get_element_type() const { return *m_element_type;}
  virtual std::unique_ptr<build_type> clone() const override;
private:
  std::unique_ptr<build_type> m_element_type;
public:
  std::vector<std::size_t> const m_size;
};

class pointer_type final : public build_type {
public:
  explicit pointer_type(std::unique_ptr<build_type> &&element_type);
  virtual llvm::Type *get_llvm_type() const override;
  std::string_view decorated_name() const;
  virtual std::unique_ptr<build_type> clone() const override;
  build_type &get_element_type() const{
    return *m_element_type;
  }
private:
  std::unique_ptr<build_type> m_element_type;
};

class reference_type final : public build_type {
public:
  explicit reference_type(std::unique_ptr<build_type> &&element_type);
  virtual llvm::Type *get_llvm_type() const override;
  std::string_view decorated_name() const;
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_element_type;
};

class mut_ast;
std::unique_ptr<build_type>
build_mut_type(base_build &father, mut_ast const &ast);

class volatile_ast;
std::unique_ptr<build_type>
build_volatile_type(base_build &father, volatile_ast const &ast);

class pointer_ast;
std::unique_ptr<build_type>
build_pointer_type(base_build &father, pointer_ast const &ast);

class reference_ast;
std::unique_ptr<reference_type>
build_reference_type(base_build &father, reference_ast const &ast);

class named_type_ast;
std::unique_ptr<build_type>
build_named_type(base_build &father, named_type_ast const &ast);
} // namespace pache

#endif
