#ifndef AST_LITERAL_H
#define AST_LITERAL_H

#include "expression.h"
#include <memory>
#include <string>
#include <string_view>
#include <utility>

namespace pache {
class build_variable;
class literal_ast : public exp_ast {
public:
  explicit literal_ast() = default;
  virtual ~literal_ast() = 0;
  
protected:
  literal_ast(literal_ast const &other) = default;
  literal_ast &operator=(literal_ast const &other) = default;
  literal_ast(literal_ast &&) = default;
  literal_ast &operator=(literal_ast &&) = default;


};

inline namespace intergral {


class intergral_literal : public literal_ast {
public:
  explicit intergral_literal(std::string &&l, std::string &&s);
  std::string const m_literal;
  std::string const m_suffix;
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
class binary_integer_literal final : public intergral_literal {
public:
  explicit binary_integer_literal(std::string &&l, std::string &&s);
  explicit binary_integer_literal(std::string &&l);

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};

class octal_integer_literal final : public intergral_literal {
public:
  explicit octal_integer_literal(std::string &&l, std::string &&s);
  explicit octal_integer_literal(std::string &&l);

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};class decimal_integer_literal final : public intergral_literal {
public:
  explicit decimal_integer_literal(std::string &&l, std::string &&s);
  explicit decimal_integer_literal(std::string &&l);

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};
class hexadecimal_integer_literal final : public intergral_literal {
public:
  explicit hexadecimal_integer_literal(std::string &&l, std::string &&s);
  explicit hexadecimal_integer_literal(std::string &&l);

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;
};

}
inline namespace boolean {
class boolean_literal final : public literal_ast {
public:
  explicit boolean_literal(bool l) : m_literal(l){}

  virtual std::unique_ptr<build_variable>
  build(base_build &build) const override;
  virtual void print() const override;  
  bool get_literal() const {
    return m_literal;
  }
private:
  bool m_literal;
};
}
} // namespace pache

#endif
