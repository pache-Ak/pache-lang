#ifndef LITERAL
#define LITERAL

#include "../IRbuild/variable.h"
#include "expression.h"
#include <memory>
#include <string_view>

namespace pache {
class literal : public exp_ast {
public:
  explicit literal() = default;
  virtual ~literal() = 0;

protected:
  literal(literal const &other) = default;
  literal &operator=(literal const &other) = default;
};

// maybe need in template
class void_literal_t final : public literal {
public:
  virtual std::unique_ptr<build_variable const>
  build(base_build &build) const override {
    return nullptr;
  }
  static void_literal_t make_void_literal_t() { return void_literal_t(); }

private:
  explicit void_literal_t() {}
};

// inline void_literal_t void_literal = void_literal_t::make_void_literal_t();
class binary_integer_literal final : public literal {
public:
  explicit binary_integer_literal(std::string_view l, std::string_view s): literal(l), suffix(s) {}

  virtual std::unique_ptr<build_variable const>
  build(base_build &build) const override;

private:
std::string_view literal;
std::string_view suffix;
};class octal_integer_literal final : public literal {
public:
  explicit octal_integer_literal(std::string_view l, std::string_view s): literal(l), suffix(s) {}

  virtual std::unique_ptr<build_variable const>
  build(base_build &build) const override;

private:
std::string_view literal;
std::string_view suffix;
};class decimal_integer_literal final : public literal {
public:
  explicit decimal_integer_literal(std::string_view l, std::string_view s): literal(l), suffix(s) {}

  virtual std::unique_ptr<build_variable const>
  build(base_build &build) const override;

private:
std::string_view literal;
std::string_view suffix;
};
class hexadecimal_integer_literal final : public literal {
public:
  explicit hexadecimal_integer_literal(std::string_view l, std::string_view s): literal(l), suffix(s) {}

  virtual std::unique_ptr<build_variable const>
  build(base_build &build) const override;

private:
std::string_view literal;
std::string_view suffix;
};
} // namespace pache

#endif
