#ifndef VARIABLE_H
#define VARIABLE_H

#include "llvm/IR/Instructions.h"

namespace pache {
class build_type;
class build_variable {
public:
  explicit build_variable(build_type *type, llvm::AllocaInst *value)
      : m_type(type), m_value(value) {}

  build_type *get_type() const { return m_type; }
  llvm::AllocaInst *get_value() const { return m_value; }

private:
  build_type *m_type;
  llvm::AllocaInst *m_value;
};
} // namespace pache

#endif
