#ifndef LITERAL
#define LITERAL

#include "../IRbuild/type.h"
#include "expression.h"
#include "type.h"
#include <memory>

namespace pache {
class literal : public exp_ast {
public:
  virtual ~literal() = 0;

protected:
  explicit literal() {}
};

// maybe need in template
class void_literal_t : public literal {
public:
  // virtual llvm::Value *codegen() override;
  //  virtual type_ast const *get_type() override { return void_type_t::get(); }
  virtual std::unique_ptr<build_variable> build() const override {
    return nullptr;
  }
  static void_literal_t make_void_literal_t() { return void_literal_t(); }
  virtual ~void_literal_t() = default;

private:
  explicit void_literal_t() {}
};

// inline void_literal_t void_literal = void_literal_t::make_void_literal_t();

class i32_literal_t : public literal {
public:
  explicit i32_literal_t(int32_t value) : m_value(value) {}

  //  virtual type_ast const *get_type() override { return i32_type_t::get(); }

  // virtual llvm::Value *codegen() override;
  virtual std::unique_ptr<build_variable> build() const override;
  virtual ~i32_literal_t() override = default;

private:
  int32_t m_value;
};
} // namespace pache

#endif
