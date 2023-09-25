#ifndef IR_TYPE_H
#define IR_TYPE_H

#include "../ast/type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>
using namespace std::literals;
namespace pache {

class build_type {
public:
  virtual bool is_integral() const { return false; }
  virtual bool is_signed() const { return false; }
  virtual bool is_unsigned() const { return false; }
  virtual std::string const decorated_name() const { return ""s; }
  virtual llvm::Type *get_llvm_type() const = 0;
  virtual void set_const() = 0;
  virtual void set_volatile() = 0;
  virtual bool is_const() const { return m_is_const; }
  virtual bool is_volatile() const { return m_is_volatile; }
  virtual ~build_type() = default;

protected:
  explicit build_type() : m_is_const(false), m_is_volatile(false) {}

  bool m_is_const;
  bool m_is_volatile;
};
std::unique_ptr<build_type> type_build(base_build *const father,
                                       type_ast const *const ast);
inline namespace primary {
class primary_type : public build_type {
public:
  virtual void set_const() override;
  virtual void set_volatile() override;
};

class void_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_void"s; }

private:
  //  explicit void_type_t() = default;
  static void_type_t void_type;
};

class bool_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_bool"s; }

private:
  //  explicit bool_type_t() = default;
  static bool_type_t bool_type;
};

class size_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_size"s; }

private:
  //  explicit size_type_t() = default;
  static size_type_t size_type;
};
inline namespace integral {
class integral_type : public primary_type {
public:
  virtual bool is_integral() const override { return true; }
};

class signed_type : public integral_type {
public:
  virtual bool is_signed() const override { return true; }
};

class unsigned_type : public integral_type {
public:
  virtual bool is_unsigned() const override { return true; }
};

class i8_type_t : public signed_type {
public:
  static void *operator new(std::size_t sz) { return &i8_type; }
  static void operator delete(void *ptr) {}
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_i8"s; }
  virtual ~i8_type_t() = default;

private:
  //  explicit i8_type_t() = default;
  static i8_type_t i8_type;
};

class i16_type_t : public signed_type {
public:
  static void *operator new(std::size_t sz) { return &i16_type; }
  static void operator delete(void *ptr) {}
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_16"s; }

private:
  //  explicit i16_type_t() = default;
  static i16_type_t i16_type;
};
class i32_type_t : public signed_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_i32"s; }

private:
  //  explicit i32_type_t() = default;
  static i32_type_t i32_type;
};

class i64_type_t : public signed_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_i64"s; }

private:
  //  explicit i64_type_t() = default;
  static i64_type_t i64_type;
};

class i128_type_t : public signed_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_i128"s; }

private:
  //  explicit i128_type_t() = default;
  static i128_type_t i128_type;
};

class u8_type_t : public unsigned_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_u8"s; }

private:
  //  explicit u8_type_t() = default;
  static u8_type_t u8_type;
};

class u16_type_t : public unsigned_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_u16"s; }

private:
  //  explicit u16_type_t() = default;
  static u16_type_t u16_type;
};
class u32_type_t : public unsigned_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_u32"s; }

private:
  //  explicit u32_type_t() = default;
  static u32_type_t u32_type;
};

class u64_type_t : public unsigned_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_u64"s; }

private:
  //  explicit u64_type_t() = default;
  static u64_type_t u64_type;
};

class u128_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_u128"s; }

private:
  //  explicit u128_type_t() = default;
  static u128_type_t u128_type;
};
} // namespace integral

inline namespace floating_pointer {

class floating_pointer_type : public primary_type {
public:
  virtual std::string const decorated_name() const override = 0;
};

class f16_type_t : public floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_f16"s; }

private:
  //  explicit f16_type_t() = default;
  static f16_type_t f16_type;
};

// a 16-bit bfloat type. iee754like
class bfloat_type_t : public floating_pointer_type {};

class f32_type_t : public floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_f32"s; }

private:
  //  explicit f32_type_t() = default;
  static f32_type_t f32_type;
};

class f64_type_t : public floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_f64"s; }

private:
  //  explicit f64_type_t() = default;
  static f64_type_t f64_type;
};

// x86 long double
class x86_fp80_type_t : public floating_pointer_type {};

class f128_type_t : public floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_f128"s; }

private:
  //  explicit f128_type_t() = default;
  static f128_type_t f128_type;
};

// powerpc long double
class ppc_fp128_type_t : public floating_pointer_type {};

inline namespace decimal_floating_pointer {
class decimal_floating_pointer_type : public floating_pointer_type {
public:
  virtual ~decimal_floating_pointer_type() = 0;
};

class d32_type_t : public decimal_floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_d32"s; }

private:
  //  explicit d32_type_t() = default;
  static d32_type_t d32_type;
};

class d64_type_t : public decimal_floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_d64"s; }

private:
  //  explicit d64_type_t() = default;
  static d64_type_t d64_type;
};

class d128_type_t : public decimal_floating_pointer_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
};
} // namespace decimal_floating_pointer

} // namespace floating_pointer

class c8_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_c8"s; }

private:
  //  explicit c8_type_t() = default;
  static c8_type_t c8_type;
};

class c16_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_c16"s; }

private:
  //  explicit c16_type_t() = default;
  static c16_type_t c16_type;
};
class c32_type_t : public primary_type {
public:
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_c32"s; }

private:
  //  explicit c32_type_t() = default;
  static c32_type_t c32_type;
};

} // namespace primary

class arr_type : public build_type {
public:
  explicit arr_type(std::unique_ptr<build_type> &&element_type,
                    const std::size_t size)
      : m_element_type(std::move(element_type)), m_size(size) {}
  virtual llvm::Type *get_llvm_type() const override;

  // std::unique_ptr<build_type> get() { return std::unique_ptr<arr_type>(this);
  // }
  virtual std::string const decorated_name() const override {
    return "_TODO_arr"s;
  }
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }

  virtual ~arr_type() = default;

private:
  std::unique_ptr<build_type> m_element_type;
  const std::size_t m_size;
};

class multi_array_type : public build_type {
public:
  explicit multi_array_type(std::unique_ptr<build_type> &&element_type,
                            std::vector<std::size_t> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}
  virtual llvm::Type *get_llvm_type() const override { return nullptr; }
  virtual std::string const decorated_name() const override {
    return "_TODO_mulri_arr"s;
  }
  virtual void set_const() override {
    std::cerr << "array type can't set const.\n";
  }
  virtual void set_volatile() override {
    std::cerr << "array type can't set volatile.\n";
  }
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }

private:
  std::unique_ptr<build_type> m_element_type;
  std::vector<std::size_t> const m_size;
};

class pointer_type : public build_type {
  virtual llvm::Type *get_llvm_type() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }

private:
  std::unique_ptr<build_type> m_element_type;
};

class reference_type : public build_type {
public:
  explicit reference_type(std::unique_ptr<build_type> &&element_type)
      : m_element_type(std::move(element_type)) {}
  virtual void set_const() override {
    std::cerr << "reference type can't set const.\n";
  }
  virtual void set_volatile() override {
    std::cerr << "reference type can't set volatile.\n";
  }
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual llvm::Type *get_llvm_type() const override {
    return llvm::PointerType::getUnqual(m_element_type->get_llvm_type());
  }

private:
  std::unique_ptr<build_type> m_element_type;
};

std::unique_ptr<reference_type>
build_reference_type(base_build *const father, reference_ast const *const ast);

std::unique_ptr<build_type> build_const_type(base_build *const father,
                                             const_type const *const ast);
std::unique_ptr<build_type> build_volatile_type(base_build *const father,
                                                volatile_type const *const ast);
} // namespace pache

#endif
