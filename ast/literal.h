#ifndef LITERAL
#define LITERAL

#include "type.h"
#include "expression.h"

#include <memory>

namespace pache {
  class literal : public exp_ast {
  public:

  protected:

  };

  class void_literal_t : public literal {
  public:

    virtual std::string dump() override {
      return "";
    }

    static void_literal_t make_void_literal_t() {
      return void_literal_t();
    }

    virtual ~void_literal_t() override = default;
  private:
    explicit void_literal_t() {
      set_type(void_type_t::get_void_type());
    }
  };

  inline void_literal_t void_literal = void_literal_t::make_void_literal_t();

  class i32_literal : public exp_ast {
  public:
    explicit i32_literal(int32_t value) : m_value(value) {
      set_type(i32_type_t::get_i32_type());
     }

    virtual std::string dump() override {
      return std::to_string(m_value);
    }



    virtual ~i32_literal() override = default;
  private:
    int32_t m_value;
  };
}

#endif
