#ifndef LITERAL
#define LITERAL

#include "expression.h"
#include "type.h"

#include <memory>

namespace pache {
class literal : public exp_ast {
public:
protected:
  explicit literal() {}
};

class void_literal_t : public literal {
public:
  virtual llvm::Value *codegen() override;
  //  virtual type_ast const *get_type() override { return void_type_t::get(); }

  static void_literal_t make_void_literal_t() { return void_literal_t(); }

  virtual ~void_literal_t() override = default;

private:
  explicit void_literal_t() {}
};

inline void_literal_t void_literal = void_literal_t::make_void_literal_t();

class i32_literal : public literal {
public:
  explicit i32_literal(int32_t value) : m_value(value) {}

  //  virtual type_ast const *get_type() override { return i32_type_t::get(); }

  virtual llvm::Value *codegen() override;

  virtual ~i32_literal() override = default;

private:
  int32_t m_value;
};
} // namespace pache

#endif
