#ifndef TYPE_H
#define TYPE_H

#include <string>

namespace pache {
  enum class value_category {
    prvalue,
    xvalue,
    lvalue,
  };

  class type_ast {
  public:
    virtual std::string dump() const = 0;

  protected:
    explicit type_ast() = default;
  };


  class primary_type : public type_ast {
  public:

  protected:
    explicit primary_type() = default;
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

  class arr_type_t : public type_ast {
  public:
    explicit arr_type_t(const type_ast * element_type, std::size_t size)
      : m_element_type(element_type), m_size(size) { }
    virtual std::string dump() const override {
      return "";
    }

    const arr_type_t *get() {
      return this;
    }

  private:
    const type_ast *m_element_type;
    std::size_t m_size;
  };
}



#endif
