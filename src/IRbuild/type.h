#ifndef IR_TYPE_H
#define IR_TYPE_H

#include "../ast/type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <memory>
#include <type_traits>
#include <utility>
#include <vector>

namespace pache {
class build_type {
public:
  virtual bool is_integral() const;
  virtual bool is_signed() const;
  virtual bool is_unsigned() const;
  virtual bool is_floating_point() const;
  virtual bool is_decimal_floating_pointer() const;
  virtual bool is_character() const;
  virtual std::string const decorated_name() const;
  virtual llvm::Type *get_llvm_type() const = 0;
  virtual void set_const() = 0;
  virtual void set_volatile() = 0;
  virtual bool is_const() const;
  virtual bool is_volatile() const;
  virtual ~build_type() = 0;
  virtual bool is_reference() { return false; }
  explicit build_type() : m_is_const(false), m_is_volatile(false) {}

  virtual std::unique_ptr<build_type> clone() const = 0;

protected:
  build_type(build_type const &other) = default;
  build_type(build_type &&other) = default;
  build_type &operator=(build_type const &other) = default;
  build_type &operator=(build_type &&other) = default;
  bool m_is_const;
  bool m_is_volatile;
};

std::unique_ptr<build_type> type_build(base_build &father, type_ast const &ast);

inline namespace primary {
class primary_type : public build_type {
public:
  virtual void set_const() override;
  virtual void set_volatile() override;
  primary_type() = default;

protected:
  primary_type(primary_type const &other) = default;
  primary_type(primary_type &&other) = default;
  primary_type &operator=(primary_type const &other) = default;
  primary_type &operator=(primary_type &&other) = default;
};

class void_type_t final : public primary_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class bool_type_t final : public primary_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class size_type_t final : public primary_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

inline namespace integral {
class integral_type : public primary_type {
public:
  virtual bool is_integral() const override { return true; }

protected:
  integral_type(integral_type const &other) = default;
  integral_type &operator=(integral_type const &other) = default;
};

class signed_type : public integral_type {
public:
  virtual bool is_signed() const override { return true; }

protected:
  signed_type(signed_type const &other) = default;
  signed_type &operator=(signed_type const &other) = default;
};

class unsigned_type : public integral_type {
public:
  virtual bool is_unsigned() const override { return true; }

protected:
  unsigned_type(unsigned_type const &other) = default;
  unsigned_type &operator=(unsigned_type const &other) = default;
};

class i8_type_t final : public signed_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class i16_type_t final : public signed_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class i32_type_t final : public signed_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class i64_type_t final : public signed_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class i128_type_t final : public signed_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class u8_type_t final : public unsigned_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class u16_type_t final : public unsigned_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class u32_type_t final : public unsigned_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class u64_type_t final : public unsigned_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class u128_type_t final : public primary_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};
} // namespace integral

inline namespace floating_pointer {

class floating_pointer_type : public primary_type {
public:
  virtual bool is_floating_point() const override;

protected:
  floating_pointer_type(floating_pointer_type const &other) = default;
  floating_pointer_type &
  operator=(floating_pointer_type const &other) = default;
};

class f16_type_t final : public floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

// a 16-bit bfloat type. iee754like
// class bfloat_type_t final : public floating_pointer_type {};

class f32_type_t final : public floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class f64_type_t final : public floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

// x86 long double
// class x86_fp80_type_t final : public floating_pointer_type {};

class f128_type_t final : public floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

// powerpc long double
// class ppc_fp128_type_t final : public floating_pointer_type {};

inline namespace decimal_floating_pointer {
class decimal_floating_pointer_type : public floating_pointer_type {
public:
  virtual bool is_decimal_floating_pointer() const override;

protected:
  decimal_floating_pointer_type(decimal_floating_pointer_type const &other) =
      default;
  decimal_floating_pointer_type &
  operator=(decimal_floating_pointer_type const &other) = default;
};

class d32_type_t final : public decimal_floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class d64_type_t final : public decimal_floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class d128_type_t final : public decimal_floating_pointer_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};
} // namespace decimal_floating_pointer

} // namespace floating_pointer

inline namespace character {
class character_type : public primary_type {
public:
  virtual bool is_character() const override { return true; }

protected:
  character_type(character_type const &other) = default;
  character_type &operator=(character_type const &other) = default;
};

class c8_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};

class c16_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};
class c32_type_t final : public character_type {
public:
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual std::unique_ptr<build_type> clone() const override;
};
} // namespace character
} // namespace primary

class arr_type final : public build_type {
public:
  explicit arr_type(std::unique_ptr<build_type> &&element_type,
                    const std::size_t size)
      : m_element_type(std::move(element_type)), m_size(size) {}
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_element_type;
  const std::size_t m_size;
};

class multi_array_type final : public build_type {
public:
  explicit multi_array_type(std::unique_ptr<build_type> &&element_type,
                            std::vector<std::size_t> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}
  explicit multi_array_type(std::unique_ptr<build_type> &&element_type,
                            std::vector<std::size_t> const&size)
      : m_element_type(std::move(element_type)), m_size(size) {}    
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_element_type;
  std::vector<std::size_t> const m_size;
};

class pointer_type final : public build_type {
public:
  explicit pointer_type(std::unique_ptr<build_type> &&element_type);
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_element_type;
};

class reference_type final : public build_type {
public:
  explicit reference_type(std::unique_ptr<build_type> &&element_type);
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual bool is_reference() override final { return true; }
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_element_type;
};


std::unique_ptr<build_type>
build_const_type(base_build &father, const_ast const &ast);

std::unique_ptr<build_type>
build_volatile_type(base_build &father, volatile_ast const &ast);

std::unique_ptr<build_type>
build_pointer_type(base_build &father, pointer_ast const &ast);

std::unique_ptr<reference_type>
build_reference_type(base_build &father, reference_ast const &ast);

std::unique_ptr<arr_type>
build_arr_type(base_build &father, arr_ast const &ast);

std::unique_ptr<multi_array_type>
build_multi_array_type(base_build &father, multi_array_ast const &ast);

std::unique_ptr<build_type>
build_named_type(base_build &father, named_ast const &ast);
} // namespace pache

#endif
