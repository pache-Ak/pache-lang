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

  class void_literal : public exp_ast {
  public:

    virtual std::string dump() const override {
      return "";
    }

    static void_literal make_void_literal() {
      return void_literal();
    }

    virtual ~void_literal() override = default;
  private:
    explicit void_literal() {
      set_type(void_type);
    }
  };

  inline void_literal void_l = void_literal::make_void_literal();

  class i32_literal : public exp_ast {
  public:
    explicit i32_literal(int32_t value) : m_value(value) {
      set_type(i32_type);
     }

    virtual std::string dump() const override {
      return std::to_string(m_value);
    }



    virtual ~i32_literal() override = default;
  private:
    int32_t m_value;
  };
}

#endif
