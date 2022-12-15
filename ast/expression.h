#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast.h"
#include "type.h"
#include <vector>
#include <cstdint>
#include <memory>
namespace pache {
  class exp_ast : public base_ast {
  public:
    explicit exp_ast() = default;

    virtual ~exp_ast() override { }

    void set_type(type_ast const *type) {
      m_type = type;
    }

    const type_ast *get_type() {
      return m_type;
    }
  protected:
    explicit exp_ast(const type_ast *type) : m_type(type) { }
    const type_ast *m_type;
  };

  enum class Operator {
  Add,
  AddressOf,
  And,
  As,
  BitwiseAnd,
  BitwiseOr,
  BitwiseXor,
  BitShiftLeft,
  BitShiftRight,
  Complement,
  Deref,
  Div,
  Eq,
  Less,
  LessEq,
  Greater,
  GreaterEq,
  Mul,
  Mod,
  Neg,
  Not,
  NotEq,
  Or,
  Sub,
  Ptr,
};

  class unary_plus : public exp_ast {
    public:
    explicit unary_plus(exp_ast* arguments)
      : exp_ast(), m_arguments(arguments) {}





    virtual ~unary_plus() = default;
  private:
    exp_ast* m_arguments;
  };

  class unary_minus : public exp_ast {
    public:
    explicit unary_minus(exp_ast* arguments)
      : exp_ast(), m_arguments(arguments) {}





    virtual ~unary_minus() = default;
  private:
    exp_ast* m_arguments;
  };
  class func_call_exp : public exp_ast {
  public:
    explicit func_call_exp(std::string &&name, std::vector<exp_ast*> &&args)
      : m_name(name), m_args(args) { }



  private:
    std::string m_name;
    std::vector<exp_ast*> m_args;
  };



  class var_exp : public exp_ast {
  public:
    explicit var_exp(std::string &&name) : m_name(name) { }
    variable_ast *get_var() const {
      return find_dec(m_name);
    }


  private:
    std::string m_name;
  };

  class binary_mul_exp : public exp_ast {
  public:
    explicit binary_mul_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~binary_mul_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_div_exp : public exp_ast {
  public:
    explicit binary_div_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~binary_div_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_mod_exp : public exp_ast {
  public:
    explicit binary_mod_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~binary_mod_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_plus_exp : public exp_ast {
  public:
    explicit binary_plus_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~binary_plus_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_minus_exp : public exp_ast {
  public:
    explicit binary_minus_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~binary_minus_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class three_way_exp : public exp_ast {
  public:
    explicit three_way_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~three_way_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class less_exp : public exp_ast {
  public:
    explicit less_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~less_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class less_eq_exp : public exp_ast {
  public:
    explicit less_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~less_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class greater_exp : public exp_ast {
  public:
    explicit greater_exp(exp_ast *arg1, exp_ast *arg2)
    : exp_ast(bool_type_t::get()), m_arg1(arg1), m_arg2(arg2) { }



    virtual ~greater_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class greater_eq_exp : public exp_ast {
  public:
    explicit greater_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~greater_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class eq_exp : public exp_ast {
  public:
    explicit eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class not_eq_exp : public exp_ast {
  public:
    explicit not_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~not_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class logical_and_exp : public exp_ast {
  public:
    explicit logical_and_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~logical_and_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class logical_or_exp : public exp_ast {
  public:
    explicit logical_or_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }



    virtual ~logical_or_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };
}

#endif
