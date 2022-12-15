#ifndef TYPE_H
#define TYPE_H

#include <string>
#include <vector>
#include <algorithm>
#include <iterator>

namespace pache {
  enum class value_category {
    prvalue,
    xvalue,
    lvalue,
  };

  class type_ast {
  public:
    virtual std::string dump() const { return ""; };

  protected:
    explicit type_ast() = default;
  };


  class primary_type : public type_ast {
  public:

  protected:
    explicit primary_type() = default;
  };

  class i8_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i8";
    }

    static i8_type_t const *get() {
      return &i8_type;
    }
  private:
    explicit i8_type_t() = default;
    static const i8_type_t i8_type;
  };

  class i16_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i16";
    }

    static i16_type_t const *get() {
      return &i16_type;
    }
  private:
    explicit i16_type_t() = default;
    static const i16_type_t i16_type;
  };
  class i32_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i32";
    }

    static i32_type_t const *get() {
      return &i32_type;
    }
  private:
    explicit i32_type_t() = default;
    static const i32_type_t i32_type;
  };

  class i64_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i64";
    }

    static i64_type_t const *get() {
      return &i64_type;
    }
  private:
    explicit i64_type_t() = default;
    static const i64_type_t i64_type;
  };

  class i128_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i128";
    }

    static i128_type_t const *get() {
      return &i128_type;
    }
  private:
    explicit i128_type_t() = default;
    static const i128_type_t i128_type;
  };

  class u8_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "u8";
    }

    static u8_type_t const *get() {
      return &u8_type;
    }
  private:
    explicit u8_type_t() = default;
    static const u8_type_t u8_type;
  };

  class u16_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "u16";
    }

    static u16_type_t const *get() {
      return &u16_type;
    }
  private:
    explicit u16_type_t() = default;
    static const u16_type_t u16_type;
  };
  class u32_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "u32";
    }

    static u32_type_t const *get() {
      return &u32_type;
    }
  private:
    explicit u32_type_t() = default;
    static const u32_type_t u32_type;
  };

  class u64_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "u64";
    }

    static u64_type_t const *get() {
      return &u64_type;
    }
  private:
    explicit u64_type_t() = default;
    static const u64_type_t u64_type;
  };

  class u128_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "u128";
    }

    static u128_type_t const *get() {
      return &u128_type;
    }
  private:
    explicit u128_type_t() = default;
    static const u128_type_t u128_type;
  };

  class f16_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "f16";
    }

    static f16_type_t const *get() {
      return &f16_type;
    }
  private:
    explicit f16_type_t() = default;
    static const f16_type_t f16_type;
  };
  class f32_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "f32";
    }

    static f32_type_t const *get() {
      return &f32_type;
    }
  private:
    explicit f32_type_t() = default;
    static const f32_type_t f32_type;
  };

  class f64_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "f64";
    }

    static f64_type_t const *get() {
      return &f64_type;
    }
  private:
    explicit f64_type_t() = default;
    static const f64_type_t f64_type;
  };

  class f128_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "f128";
    }

    static f128_type_t const *get() {
      return &f128_type;
    }
  private:
    explicit f128_type_t() = default;
    static const f128_type_t f128_type;
  };

  class d32_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "d32";
    }

    static d32_type_t const *get() {
      return &d32_type;
    }
  private:
    explicit d32_type_t() = default;
    static const d32_type_t d32_type;
  };

  class d64_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "d64";
    }

    static d64_type_t const *get() {
      return &d64_type;
    }
  private:
    explicit d64_type_t() = default;
    static const d64_type_t d64_type;
  };

  class c8_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "c8";
    }

    static c8_type_t const *get() {
      return &c8_type;
    }
  private:
    explicit c8_type_t() = default;
    static const c8_type_t c8_type;
  };

  class c16_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "c16";
    }

    static c16_type_t const *get() {
      return &c16_type;
    }
  private:
    explicit c16_type_t() = default;
    static const c16_type_t c16_type;
  };
  class c32_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "c32";
    }

    static c32_type_t const *get() {
      return &c32_type;
    }
  private:
    explicit c32_type_t() = default;
    static const c32_type_t c32_type;
  };

  class void_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "void";
    }
    static void_type_t const *get() {
      return &void_type;
    }
  private:
    explicit void_type_t() = default;
    static const void_type_t void_type;
  };

  class bool_type_t : public primary_type {
  public:
    virtual std::string dump() const override {
      return "i1";
    }
    static bool_type_t const *get() {
      return &bool_type;
    }
  private:
    explicit bool_type_t() = default;
    static const bool_type_t bool_type;
  };

  class size_type_t : public primary_type {
  public:
    static size_type_t const * get() {
      return &size_type;
    }

  private:
    explicit size_type_t() = default;
    static const size_type_t size_type;
  };

  class arr_type_t : public type_ast {
  public:
    explicit arr_type_t(const type_ast * element_type, const std::size_t size)
      : m_element_type(element_type), m_size(size) { }
    virtual std::string dump() const override {
      return "";
    }

    const arr_type_t *get() const {
      return this;
    }

  private:
    const type_ast *m_element_type;
    const std::size_t m_size;
  };
  class variable_ast;
  class func_type : public type_ast {
  public:
    explicit func_type(const std::vector<variable_ast*> &args, const type_ast* return_type);


    virtual std::string dump() const override {
      return "";
    }

    const func_type *get() const {
      return this;
    }
  private:
    std::vector<const type_ast*> m_args_type;
    const type_ast *m_return_type;
  };
  class class_ast;
  class class_type : public type_ast {
  public:
    explicit class_type(const class_ast *class_def) : m_def(class_def) { }

    const class_type *get() const {
      return this;
    }
  private:
    const class_ast *m_def;
  };

  class compunit_ast;

  class user_def_type : public type_ast {
  public:
    explicit user_def_type(std::string &&identifier)
      : m_iden(std::move(identifier)) { }

    type_ast *get(compunit_ast *comp);

  private:
    std::string m_iden;
  };
}



#endif
