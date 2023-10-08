#ifndef IR_TYPE_H
#define IR_TYPE_H

#include "../ast/type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <memory>
#include <utility>
#include <vector>

namespace pache {

class build_type {
public:
  virtual bool is_integral() const;
  virtual bool is_signed() const;
  virtual bool is_unsigned() const;
  virtual std::string const decorated_name() const;
  virtual llvm::Type *get_llvm_type() const = 0;
  virtual void set_const() = 0;
  virtual void set_volatile() = 0;
  virtual bool is_const() const;
  virtual bool is_volatile() const;
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
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit void_type_t() = default;
};

class bool_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit bool_type_t() = default;
};

class size_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit size_type_t() = default;
};
inline namespace integral {
class integral_type : public primary_type {
public:
  virtual bool is_integral() const override { return true; }
  virtual ~integral_type() = 0;
};

class signed_type : public integral_type {
public:
  virtual bool is_signed() const override { return true; }
  virtual ~signed_type() = 0;
};

class unsigned_type : public integral_type {
public:
  virtual bool is_unsigned() const override { return true; }
  virtual ~unsigned_type() = 0;
};

class i8_type_t : public signed_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual ~i8_type_t() = default;

private:
  explicit i8_type_t() = default;
};

class i16_type_t : public signed_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit i16_type_t() = default;
};
class i32_type_t : public signed_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit i32_type_t() = default;
};

class i64_type_t : public signed_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit i64_type_t() = default;
};

class i128_type_t : public signed_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit i128_type_t() = default;
};

class u8_type_t : public unsigned_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit u8_type_t() = default;
};

class u16_type_t : public unsigned_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit u16_type_t() = default;
};
class u32_type_t : public unsigned_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit u32_type_t() = default;
};

class u64_type_t : public unsigned_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit u64_type_t() = default;
};

class u128_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit u128_type_t() = default;
};
} // namespace integral

inline namespace floating_pointer {

class floating_pointer_type : public primary_type {
public:
  virtual std::string const decorated_name() const override = 0;
  virtual ~floating_pointer_type() = 0;
};

class f16_type_t : public floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit f16_type_t() = default;
};

// a 16-bit bfloat type. iee754like
class bfloat_type_t : public floating_pointer_type {};

class f32_type_t : public floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit f32_type_t() = default;
};

class f64_type_t : public floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit f64_type_t() = default;
};

// x86 long double
class x86_fp80_type_t : public floating_pointer_type {};

class f128_type_t : public floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit f128_type_t() = default;
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
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_d32"s; }

private:
  //  explicit d32_type_t() = default;
  static d32_type_t d32_type;
};

class d64_type_t : public decimal_floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override { return "_d64"s; }

private:
  //  explicit d64_type_t() = default;
  static d64_type_t d64_type;
};

class d128_type_t : public decimal_floating_pointer_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
};
} // namespace decimal_floating_pointer

} // namespace floating_pointer

class c8_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit c8_type_t() = default;
};

class c16_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit c16_type_t() = default;
};
class c32_type_t : public primary_type {
public:
  void *operator new(std::size_t count);
  void operator delete(void *ptr);
  static std::unique_ptr<build_type> get();
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

private:
  explicit c32_type_t() = default;
};

} // namespace primary

class arr_type : public build_type {
public:
  explicit arr_type(std::unique_ptr<build_type> &&element_type,
                    const std::size_t size)
      : m_element_type(std::move(element_type)), m_size(size) {}
  virtual llvm::Type *get_llvm_type() const override;
  //  std::unique_ptr<build_type>  get();
  virtual std::string const decorated_name() const override;
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
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;
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
  virtual std::string const decorated_name() const override;
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
  virtual void set_const() override;
  virtual void set_volatile() override;
  virtual bool is_const() const override { return m_element_type->is_const(); }
  virtual bool is_volatile() const override {
    return m_element_type->is_volatile();
  }
  virtual llvm::Type *get_llvm_type() const override;
  virtual std::string const decorated_name() const override;

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
