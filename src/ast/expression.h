#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../IRbuild/variable.h"
#include "ast.h"
#include "scope.h"
#include "type.h"
#include <memory>
#include <string_view>
#include <utility>

namespace pache {
class exp_ast : public base_ast {
public:
  explicit exp_ast() = default;
  virtual ~exp_ast() = 0;

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const = 0;

protected:
  exp_ast(exp_ast const &other) = default;
  exp_ast &operator=(exp_ast const &other) = default;
  exp_ast(exp_ast &&) = default;
  exp_ast &operator=(exp_ast &&) = default;
};
/* enum class Operator {
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
}; */

class unary_plus final : public exp_ast {
public:
  explicit unary_plus(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};

class unary_minus final : public exp_ast {
public:
  explicit unary_minus(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};
class func_call_exp final : public exp_ast {
public:
  explicit func_call_exp(std::unique_ptr<exp_ast> &&func,
                         std::vector<std::unique_ptr<exp_ast>> &&args);
  //std::string const &get_name() const { return m_name; }
  std::vector<std::unique_ptr<exp_ast>> const &get_args() const;
  exp_ast const &get_func() const;
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_exp;
  std::vector<std::unique_ptr<exp_ast>> m_args;
};

class var_exp final : public exp_ast {
public:
  explicit var_exp(named_ast &&ast) : m_iden(std::move(ast)) {}
  explicit var_exp(std::unique_ptr<scope_ast> &&scope, std::string_view name)
    :m_iden(std::move(scope), name) {}

  std::string_view get_name() const;
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;

private:
named_ast m_iden;
};

class binary_mul_exp final : public exp_ast {
public:
  explicit binary_mul_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }

  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_div_exp final : public exp_ast {
public:
  explicit binary_div_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_mod_exp final : public exp_ast {
public:
  explicit binary_mod_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_plus_exp final : public exp_ast {
public:
  explicit binary_plus_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_minus_exp final : public exp_ast {
public:
  explicit binary_minus_exp(std::unique_ptr<exp_ast> &&lhs,
                            std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class three_way_exp final : public exp_ast {
public:
  explicit three_way_exp(std::unique_ptr<exp_ast> &&lhs,
                         std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class less_exp final : public exp_ast {
public:
  explicit less_exp(std::unique_ptr<exp_ast> &&lhs,
                    std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class less_eq_exp final : public exp_ast {
public:
  explicit less_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                       std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class greater_exp final : public exp_ast {
public:
  explicit greater_exp(std::unique_ptr<exp_ast> &&lhs,
                       std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class greater_eq_exp final : public exp_ast {
public:
  explicit greater_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class eq_exp final : public exp_ast {
public:
  explicit eq_exp(std::unique_ptr<exp_ast> &&lhs,
                  std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class not_eq_exp final : public exp_ast {
public:
  explicit not_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                      std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_and_exp final : public exp_ast {
public:
  explicit bitwise_and_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_xor_exp final : public exp_ast {
public:
  explicit bitwise_xor_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;

  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_or_exp final : public exp_ast {
public:
  explicit bitwise_or_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class logical_and_exp final : public exp_ast {
public:
  explicit logical_and_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class logical_or_exp final : public exp_ast {
public:
  explicit logical_or_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class subscript_exp final : public exp_ast {
public:
  explicit subscript_exp(std::unique_ptr<exp_ast> &&func,
                         std::vector<std::unique_ptr<exp_ast>> &&args)
      : m_function(std::move(func)), m_args(std::move(args)) {}
  //std::string const &get_name() const { return m_name; }
  std::vector<std::unique_ptr<exp_ast>> const &get_args() const {
    return m_args;
  }
  exp_ast const&get_arr() const { return *m_function;}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_function;
  std::vector<std::unique_ptr<exp_ast>> m_args;
};

class dot_exp final : public exp_ast {
public:
  dot_exp(std::unique_ptr<exp_ast> &&expr, std::string_view id_expr)
    : m_expr(std::move(expr)), m_id_expr(id_expr){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_exp() const { return *m_expr;}
  std::string_view get_id() const { return m_id_expr;}
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_expr;
  std::string_view m_id_expr;
};

class arrow_exp final : public exp_ast {
public:
  arrow_exp(std::unique_ptr<exp_ast> &&expr, std::string_view id_expr)
    : m_expr(std::move(expr)), m_id_expr(id_expr){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_expr;
  std::string_view m_id_expr;
};

class logical_not_exp final : public exp_ast {
public:
  explicit logical_not_exp(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};

class bitwise_not_exp final : public exp_ast {
public:
  explicit bitwise_not_exp(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};

class indirection_exp final : public exp_ast {
public:
  explicit indirection_exp(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};

class address_of_exp final : public exp_ast {
public:
  explicit address_of_exp(std::unique_ptr<exp_ast> &&argument)
      : m_argument(std::move(argument)) {}
  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_argument; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_argument;
};

class allocation_exp final : public exp_ast {
public:
  allocation_exp(std::unique_ptr<type_ast> &&type, std::unique_ptr<exp_ast> &&init)
    : m_type(std::move(type)), m_init(std::move(init)){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_type;
  std::unique_ptr<exp_ast> m_init;
};

class deallocation_exp final : public exp_ast {
public:
  explicit deallocation_exp(std::unique_ptr<exp_ast> &&exp)
    : m_exp(std::move(exp)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_exp; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_exp;
};

/* class allocation_array_exp final : public exp_ast {
// maybe not used
};

class deallocation_array_exp final : public exp_ast {
public:
// maybe not used

  explicit deallocation_array_exp(std::unique_ptr<exp_ast> exp);

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_arg() const { return *m_exp; }
private:
  std::unique_ptr<exp_ast> m_exp;
}; */

class left_shift_exp final : public exp_ast {
public:
  left_shift_exp(std::unique_ptr<exp_ast> &&lhs, std::unique_ptr<exp_ast> &&rhs)
    : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class right_shift_exp final : public exp_ast {
public:
  right_shift_exp(std::unique_ptr<exp_ast> &&lhs, std::unique_ptr<exp_ast> &&rhs)
    : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  exp_ast const &get_lhs() const { return *m_lhs; }
  exp_ast const &get_rhs() const { return *m_rhs; }
  virtual void print() const override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};
} // namespace pache

#endif
