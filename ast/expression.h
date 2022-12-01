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
    virtual std::string dump() override = 0;

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

    virtual std::string dump() override {
        m_arguments->set_father(m_father);
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

    virtual std::string dump() override {
      m_arguments->set_father(m_father);
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
  class func_call_exp : public exp_ast {
  public:
    explicit func_call_exp(std::string *name, std::vector<exp_ast*> *args)
      : m_name(name), m_args(args) { }

    virtual std::string dump() override {
      variable_ast *var = find_dec(*m_name);
      if (var != nullptr) {
        std::vector<std::string> ss;
        for (auto exp : *m_args) {
          ss.push_back(exp->dump());
        }
        std::string s{"%"};
        s += std::to_string(ssa_value);
        std::cout << s << " = call " << var->get_type()->dump()
                  << " @" << var->get_name() << "(";



        auto beg1 = m_args->begin();
        auto beg2 = ss.begin();
        if (beg1 != m_args->end()) {
          std::cout << (*beg1)->get_type()->dump() << " "
                    << *beg2;
          ++beg1;
          ++beg2;
        }
        while(beg1 !=  m_args->end()) {
          std::cout << ", " << (*beg1)->get_type()->dump() << " "
                    << *beg2;
          ++beg1;
          ++beg2;
        }

        std::cout << ")\n";
        return s;
      } else {
        std::cout << "error : name " << *m_name << "can't find.";
        return "";
      }
    }

  private:
    std::string *m_name;
    std::vector<exp_ast*> *m_args;
  };



  class var_exp : public exp_ast {
  public:
    explicit var_exp(std::string *name) : m_name(name) { }
    variable_ast *get_var() const {
      return find_dec(*m_name);
    }

    virtual std::string dump() override {
      variable_ast *var = get_var();
      if (var != nullptr) {

      set_type(var->get_type());
        std::string s{"%"};
        s += std::to_string(ssa_value);
        std::cout << s << " = load "
                  << var->get_type()->dump()
                  << ", " << var->get_type()->dump()
                  << "* @" << var->get_name() << "\n";
        ++ssa_value;
        return s;
      } else {
        std::cout << "error : name " << *m_name << "can't find.";
        return "";
      }
    }
  private:
    std::string *m_name;
  };

  class binary_mul_exp : public exp_ast {
  public:
    explicit binary_mul_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
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

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
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

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
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

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
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

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
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

  class three_way_exp : public exp_ast {
  public:
    explicit three_way_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = TODO " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~three_way_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class less_exp : public exp_ast {
  public:
    explicit less_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = le " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~less_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class less_eq_exp : public exp_ast {
  public:
    explicit less_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = le_eq " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~less_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class greater_exp : public exp_ast {
  public:
    explicit greater_exp(exp_ast *arg1, exp_ast *arg2)
    : exp_ast(bool_type_t::get()), m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = gr " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~greater_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class greater_eq_exp : public exp_ast {
  public:
    explicit greater_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = gr_eq " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~greater_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class eq_exp : public exp_ast {
  public:
    explicit eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = eq " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class not_eq_exp : public exp_ast {
  public:
    explicit not_eq_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::string s2 = m_arg2->dump();
      std::cout << "%" << ssa_value << " = not_eq " << s1 << ", " << s2 << "\n";
      std::string s{"%"};
      s += std::to_string(ssa_value);
      ++ssa_value;
      return s;
    }

    virtual ~not_eq_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class logical_and_exp : public exp_ast {
  public:
    explicit logical_and_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();
      std::size_t var = ssa_value++;

      std::cout << "%" << var << " alloca i1\n";
      std::size_t block1 = ssa_value++;
      std::size_t block2 = ssa_value++;
      std::cout << "br i1 " << s1 << ", label %" << block1
                << ", label %" << block2 << "\n"
                << block1 << ":\n";

      std::string s2 = m_arg2->dump();
      std::size_t block3 = ssa_value++;
      std::cout << "store i1 " << s2 << ", i1* %" << var << ", align 1"
                << "\nbr label %" << block3
                << "\n\n" << block2 << ":\n";
      std::cout << "store i1 " << s1 << ", i1* %" << var << ", align 1"
                << "\nbr label %" << block3
                << "\n\n" << block3 << ":\n";

      std::string s{"%"};
      s += std::to_string(var);
      return s;
    }

    virtual ~logical_and_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };

  class logical_or_exp : public exp_ast {
  public:
    explicit logical_or_exp(exp_ast *arg1, exp_ast *arg2) : m_arg1(arg1), m_arg2(arg2) { }

    virtual std::string dump() override {
      m_arg1->set_father(m_father);
      m_arg2->set_father(m_father);
      std::string s1 = m_arg1->dump();

      std::size_t var = ssa_value++;

      std::cout << "%" << var << " = alloca i1\n";
      std::size_t block1 = ssa_value++;
      std::size_t block2 = ssa_value++;
      std::cout << "br i1 " << s1 << ", label %" << block1
                << ", label %" << block2 << "\n"
                << block1 << ":\n";

      std::size_t block3 = ssa_value++;
      std::cout << "store i1 " << s1 << ", i1* %" << var << ", align 1"
                << "\nbr label %" << block3
                << "\n\n" << block2 << ":\n";

      std::string s2 = m_arg2->dump();
      std::cout << "store i1 " << s2 << ", i1* %" << var << ", align 1"
                << "\nbr label %" << block3
                << "\n\n" << block3 << ":\n";

      std::string s{"%"};
      s += std::to_string(var);
      return s;
    }

    virtual ~logical_or_exp() override = default;
  private:
    exp_ast *m_arg1;
    exp_ast *m_arg2;
  };
}

#endif
