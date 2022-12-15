#ifndef LITERAL
#define LITERAL

#include "type.h"
#include "expression.h"

#include <memory>

namespace pache {
  class literal : public exp_ast {
  public:

  protected:
    explicit literal(const type_ast *type) : exp_ast(type) { }
  };

  class void_literal_t : public literal {
  public:



    static void_literal_t make_void_literal_t() {
      return void_literal_t();
    }

    virtual ~void_literal_t() override = default;
  private:
    explicit void_literal_t() : literal(void_type_t::get()) { }
  };

  inline void_literal_t void_literal = void_literal_t::make_void_literal_t();

  class i32_literal : public literal {
  public:
    explicit i32_literal(int32_t value)
      : literal(i32_type_t::get()), m_value(value) { }





    virtual ~i32_literal() override = default;
  private:
    int32_t m_value;
  };
}

#endif
