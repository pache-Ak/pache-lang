#include "literal.h"
#include "../IRbuild/build.h"

namespace pache {
std::unique_ptr<build_variable> i32_literal_t::build() const {
  return std::make_unique<build_constant_variable>(
      i32_type_t::get(),
      llvm::ConstantInt::get(Builder->getInt32Ty(), m_value, true));
}
} // namespace pache
