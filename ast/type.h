#ifndef TYPE_H
#define TYPE_H

#include "ast.h"

namespace pache {
  enum class value_category {
    prvalue,
    xvalue,
    lvalue,
  };




  class primary_type : public type_ast {
  public:
  protected:
    explicit primary_type() { }
  };

  class i32_type_t : public primary_type {
  public:
    virtual std::string dump() override {
      return "i32";
    }
    static const i32_type_t make_i32_type() {
      return i32_type_t();
    }
  private:
    explicit i32_type_t() { }
  };
  inline i32_type_t i32_type = i32_type_t::make_i32_type();
  class void_type_t : public primary_type {
  public:
    virtual std::string dump() override {
      return "void";
    }
    static const void_type_t make_void_type() {
      return void_type_t();
    }
  };
  inline void_type_t void_type = void_type_t::make_void_type();

  class bool_type_t : public primary_type {
  public:
    virtual std::string dump() override {
      return "i1";
    }
    static bool_type_t &get_bool_type() {
      return m_instance;
    }
  private:
    explicit bool_type_t() { }
    static bool_type_t m_instance;
  };
  inline bool_type_t bool_type_t::m_instance;
  inline bool_type_t &bool_type = bool_type_t::get_bool_type();
}



#endif
