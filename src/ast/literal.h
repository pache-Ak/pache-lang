#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "../IRbuild/variable.h"
#include "expression.h"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pache {
class literal_ast : public exp_ast {
public:
  explicit literal_ast(std::string &&l, std::string &&s): m_literal(std::move(l)), m_suffix(std::move(s)) {}
  virtual ~literal_ast() = 0;
  
  std::string const m_literal;
  std::string const m_suffix;
protected:
  literal_ast(literal_ast const &other) = default;
  literal_ast &operator=(literal_ast const &other) = delete;
  literal_ast(literal_ast &&) = default;
  literal_ast &operator=(literal_ast &&) = delete;


};

// // maybe need in template
// class void_literal_t final : public literal {
// public:
//   virtual std::unique_ptr<build_variable>
//   build(base_build &build) const override {
//     return nullptr;
//   }
//   static void_literal_t make_void_literal_t() { return void_literal_t(); }

// private:
//   explicit void_literal_t() {}
// };

// inline void_literal_t void_literal = void_literal_t::make_void_literal_t();
class binary_integer_literal final : public literal_ast {
public:
  explicit binary_integer_literal(std::string&& l, std::string&& s): literal_ast(std::move(l), std::move(s)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};

class octal_integer_literal final : public literal_ast {
public:
  explicit octal_integer_literal(std::string&& l, std::string&& s): literal_ast(std::move(l), std::move(s)){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};class decimal_integer_literal final : public literal_ast {
public:
  explicit decimal_integer_literal(std::string&& l, std::string&& s): literal_ast(std::move(l), std::move(s)) {}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};
class hexadecimal_integer_literal final : public literal_ast {
public:
  explicit hexadecimal_integer_literal(std::string&& l, std::string&& s): literal_ast(std::move(l), std::move(s)){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};
} // namespace pache

#endif
