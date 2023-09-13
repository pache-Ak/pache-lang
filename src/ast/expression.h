#ifndef EXPRESSION_H
#define EXPRESSION_H

#include "../IRbuild/variable.h"
#include "ast.h"
#include "type.h"
#include <cstdint>
#include <memory>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace pache {
class exp_ast : public base_ast {
public:
  explicit exp_ast() = default;

  virtual ~exp_ast() override {}

  virtual std::unique_ptr<build_variable> build() const = 0;
};
std::unique_ptr<build_variable> build_exp(std::unique_ptr<exp_ast> const &ast);
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

class unary_plus : public exp_ast {
public:
  explicit unary_plus(std::unique_ptr<exp_ast> &&argument)
      : m_arguments({std::move(argument)}) {}
  virtual ~unary_plus() = default;
  virtual std::unique_ptr<build_variable> build() const override;

private:
  std::array<std::unique_ptr<exp_ast>, 1> m_arguments;
  // std::unique_ptr<exp_ast> m_argument;
};

class unary_minus : public exp_ast {
public:
  explicit unary_minus(std::unique_ptr<exp_ast> &&argument)
      : m_arguments({std::move(argument)}) {}

  virtual ~unary_minus() = default;
  virtual std::unique_ptr<build_variable> build() const override;

private:
  std::array<std::unique_ptr<exp_ast>, 1> m_arguments;
  // std::unique_ptr<exp_ast> m_argument;
};
class func_call_exp : public exp_ast {
public:
  explicit func_call_exp(std::string &&name,
                         std::vector<std::unique_ptr<exp_ast>> &&args)
      : m_name(std::move(name)), m_args(std::move(args)) {}
  // virtual llvm::Value *codegen() override;
  std::string const &get_name() const { return m_name; }
  std::vector<std::unique_ptr<exp_ast>> const &get_args() const {
    return m_args;
  }

private:
  std::string m_name;
  std::vector<std::unique_ptr<exp_ast>> m_args;
};

class var_exp : public exp_ast {
public:
  explicit var_exp(std::string &&name) : m_name(std::move(name)) {}

  std::string const &get_name() const { return m_name; }
  virtual std::unique_ptr<build_variable> build() const override;

private:
  std::string m_name;
};

class binary_mul_exp : public exp_ast {
public:
  explicit binary_mul_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~binary_mul_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_div_exp : public exp_ast {
public:
  explicit binary_div_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~binary_div_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_mod_exp : public exp_ast {
public:
  explicit binary_mod_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~binary_mod_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_plus_exp : public exp_ast {
public:
  explicit binary_plus_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~binary_plus_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class binary_minus_exp : public exp_ast {
public:
  explicit binary_minus_exp(std::unique_ptr<exp_ast> &&lhs,
                            std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~binary_minus_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class three_way_exp : public exp_ast {
public:
  explicit three_way_exp(std::unique_ptr<exp_ast> &&lhs,
                         std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~three_way_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class less_exp : public exp_ast {
public:
  explicit less_exp(std::unique_ptr<exp_ast> &&lhs,
                    std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~less_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class less_eq_exp : public exp_ast {
public:
  explicit less_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                       std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~less_eq_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class greater_exp : public exp_ast {
public:
  explicit greater_exp(std::unique_ptr<exp_ast> &&lhs,
                       std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~greater_exp() override = default;
  // virtual type_ast const *get_type() override { return bool_type_t::get(); }

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class greater_eq_exp : public exp_ast {
public:
  explicit greater_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~greater_eq_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class eq_exp : public exp_ast {
public:
  explicit eq_exp(std::unique_ptr<exp_ast> &&lhs,
                  std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~eq_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class not_eq_exp : public exp_ast {
public:
  explicit not_eq_exp(std::unique_ptr<exp_ast> &&lhs,
                      std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~not_eq_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_and_exp : public exp_ast {
public:
  explicit bitwise_and_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~bitwise_and_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_xor_exp : public exp_ast {
public:
  explicit bitwise_xor_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}
  // virtual llvm::Value *codegen() override;
  virtual ~bitwise_xor_exp() override = default;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class bitwise_or_exp : public exp_ast {
public:
  explicit bitwise_or_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~bitwise_or_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class logical_and_exp : public exp_ast {
public:
  explicit logical_and_exp(std::unique_ptr<exp_ast> &&lhs,
                           std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~logical_and_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

class logical_or_exp : public exp_ast {
public:
  explicit logical_or_exp(std::unique_ptr<exp_ast> &&lhs,
                          std::unique_ptr<exp_ast> &&rhs)
      : m_lhs(std::move(lhs)), m_rhs(std::move(rhs)) {}

  virtual ~logical_or_exp() override = default;
  // virtual llvm::Value *codegen() override;

private:
  std::unique_ptr<exp_ast> m_lhs;
  std::unique_ptr<exp_ast> m_rhs;
};

} // namespace pache

#endif
