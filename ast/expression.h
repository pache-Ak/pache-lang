#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "ast.h"
#include <vector>
#include <cstdint>

namespace pache {
  class exp_ast : base_ast {
  public:
    virtual std::string dump() const override = 0;

    virtual ~exp_ast() override { }


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

  class operator_ast : public exp_ast {
  public:
    explicit operator_ast(Operator op, std::vector<exp_ast*> arguments)
      : exp_ast(), m_operator(op), m_arguments(std::move(arguments)) {}

    virtual std::string dump() const override {
      std::ostringstream out;
      return out.str();
    }



    virtual ~operator_ast() = 0;
  private:
    Operator m_operator;
    std::vector<exp_ast*> m_arguments;
  };

  class unary_plus : public exp_ast {
    public:
    explicit unary_plus(exp_ast* arguments)
      : exp_ast(), m_arguments(arguments) {}

    virtual std::string dump() const override {
        return m_arguments->dump();
    }



    virtual ~unary_plus() = default;
  private:
    exp_ast* m_arguments;
  };

  class unary_minus : public exp_ast {
    public:
    explicit unary_minus(exp_ast* arguments)
      : exp_ast(), m_arguments(arguments) {}

    virtual std::string dump() const override {
      std::string s1 = m_arguments->dump();
      std::cout << "%" << ssa_value << " = sub, 0, " << s1 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }



    virtual ~unary_minus() = default;
  private:
    exp_ast* m_arguments;
  };

  class i32_literal : public exp_ast {
  public:
    explicit i32_literal(int32_t value) : m_value(value) { }

    virtual std::string dump() const override {
      return std::to_string(m_value);
    }



    virtual ~i32_literal() override = default;
  private:
    int32_t m_value;
  };

  class binary_mul_exp : public exp_ast {
  public:
    explicit binary_mul_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() const override {
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = mul " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~binary_mul_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_div_exp : public exp_ast {
  public:
    explicit binary_div_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() const override {
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = div " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~binary_div_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_mod_exp : public exp_ast {
  public:
    explicit binary_mod_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() const override {
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = mod " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~binary_mod_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

    class binary_plus_exp : public exp_ast {
  public:
    explicit binary_plus_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() const override {
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = add " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~binary_plus_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class binary_minus_exp : public exp_ast {
  public:
    explicit binary_minus_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() const override {
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = sub " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~binary_minus_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

}

#endif
