#ifndef TYPE_H
#define TYPE_H

#include <algorithm>
#include <iterator>
#include <string>
#include <type_traits>
#include <vector>

#include "llvm/IR/Value.h"

using namespace std::literals;

namespace pache {
/* enum class value_category {
  prvalue,
  xvalue,
  lvalue,
}; */

class type_ast {
public:
  virtual llvm::Type *codegen() = 0;

  virtual bool is_integral() const { return false; }

  virtual bool is_signed() const { return false; }

  virtual bool is_unsigned() const { return false; }

  virtual bool isConstant() const { return false; }

  virtual std::string const encoding_name() const = 0;

protected:
  explicit type_ast() = default;
};

template <class T> class const_type {
public:
  virtual bool isConstant() const { return true; }
};

template <class T> class const_type<const_type<T>> {
  std::enable_if<false> isConstant() const {}
};

class primary_type : public type_ast {
public:
protected:
  explicit primary_type() = default;
};

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
  static i8_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_i8"s; }

private:
  //  explicit i8_type_t() = default;
  static i8_type_t i8_type;
};

class i16_type_t : public signed_type {
public:
  static i16_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_16"s; }

private:
  //  explicit i16_type_t() = default;
  static i16_type_t i16_type;
};
class i32_type_t : public signed_type {
public:
  static i32_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_i32"s; }

private:
  //  explicit i32_type_t() = default;
  static i32_type_t i32_type;
};

class i64_type_t : public signed_type {
public:
  static i64_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_i64"s; }

private:
  //  explicit i64_type_t() = default;
  static i64_type_t i64_type;
};

class i128_type_t : public signed_type {
public:
  static i128_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_i128"s; }

private:
  //  explicit i128_type_t() = default;
  static i128_type_t i128_type;
};

class u8_type_t : public unsigned_type {
public:
  static u8_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_u8"s; }

private:
  //  explicit u8_type_t() = default;
  static u8_type_t u8_type;
};

class u16_type_t : public unsigned_type {
public:
  static u16_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_u16"s; }

private:
  //  explicit u16_type_t() = default;
  static u16_type_t u16_type;
};
class u32_type_t : public unsigned_type {
public:
  static u32_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_u32"s; }

private:
  //  explicit u32_type_t() = default;
  static u32_type_t u32_type;
};

class u64_type_t : public unsigned_type {
public:
  static u64_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_u64"s; }

private:
  //  explicit u64_type_t() = default;
  static u64_type_t u64_type;
};

class u128_type_t : public primary_type {
public:
  static u128_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_u128"s; }

private:
  //  explicit u128_type_t() = default;
  static u128_type_t u128_type;
};

class f16_type_t : public primary_type {
public:
  static f16_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_f16"s; }

private:
  //  explicit f16_type_t() = default;
  static f16_type_t f16_type;
};

// a 16-bit bfloat type. iee754like
class bfloat_type_t : public primary_type {};

class f32_type_t : public primary_type {
public:
  static f32_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_f32"s; }

private:
  //  explicit f32_type_t() = default;
  static f32_type_t f32_type;
};

class f64_type_t : public primary_type {
public:
  static f64_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_f64"s; }

private:
  //  explicit f64_type_t() = default;
  static f64_type_t f64_type;
};

// x86 long double
class x86_fp80_type_t : public primary_type {};

class f128_type_t : public primary_type {
public:
  static f128_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_f128"s; }

private:
  //  explicit f128_type_t() = default;
  static f128_type_t f128_type;
};

// powerpc long double
class ppc_fp128_type_t : public primary_type {};

class d32_type_t : public primary_type {
public:
  static d32_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_d32"s; }

private:
  //  explicit d32_type_t() = default;
  static d32_type_t d32_type;
};

class d64_type_t : public primary_type {
public:
  static d64_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_d64"s; }

private:
  //  explicit d64_type_t() = default;
  static d64_type_t d64_type;
};

class c8_type_t : public primary_type {
public:
  static c8_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_c8"s; }

private:
  //  explicit c8_type_t() = default;
  static c8_type_t c8_type;
};

class c16_type_t : public primary_type {
public:
  static c16_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_c16"s; }

private:
  //  explicit c16_type_t() = default;
  static c16_type_t c16_type;
};
class c32_type_t : public primary_type {
public:
  static c32_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_c32"s; }

private:
  //  explicit c32_type_t() = default;
  static c32_type_t c32_type;
};

class void_type_t : public primary_type {
public:
  static void_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_void"s; }

private:
  //  explicit void_type_t() = default;
  static void_type_t void_type;
};

class bool_type_t : public primary_type {
public:
  static bool_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_bool"s; }

private:
  //  explicit bool_type_t() = default;
  static bool_type_t bool_type;
};

class size_type_t : public primary_type {
public:
  static size_type_t *get();
  virtual llvm::Type *codegen() override;
  virtual std::string const encoding_name() const override { return "_size"s; }

private:
  //  explicit size_type_t() = default;
  static size_type_t size_type;
};

class arr_type_t : public type_ast {
public:
  explicit arr_type_t(type_ast *element_type, const std::size_t size)
      : m_element_type(element_type), m_size(size) {}
  virtual llvm::Type *codegen() override;

  const arr_type_t *get() const { return this; }
  virtual std::string const encoding_name() const override {
    return "_TODO_arr"s;
  }

private:
  type_ast *m_element_type;
  const std::size_t m_size;
};
class variable_ast;
class func_type : public type_ast {
public:
  explicit func_type(const std::vector<variable_ast *> &args,
                     const type_ast *return_type);

  const func_type *get() const { return this; }
  virtual std::string const encoding_name() const override {
    return "_TODO_func"s;
  }

private:
  std::vector<const type_ast *> m_args_type;
  const type_ast *m_return_type;
};
class class_ast;
class class_type : public type_ast {
public:
  explicit class_type(const class_ast *class_def) : m_def(class_def) {}
  virtual llvm::Type *codegen() override;

  const class_type *get() const { return this; }
  virtual std::string const encoding_name() const override {
    return "_TODO_class"s;
  }

private:
  const class_ast *m_def;
};

class compunit_ast;

class user_def_type : public type_ast {
public:
  explicit user_def_type(std::string &&identifier)
      : m_iden(std::move(identifier)) {}

  type_ast *get(compunit_ast *comp);
  virtual llvm::Type *codegen() override;
  virtual ~user_def_type() = default;
  virtual std::string const encoding_name() const override {
    return "_TODO_class"s;
  }

private:
  std::string m_iden;
};
} // namespace pache

#endif
