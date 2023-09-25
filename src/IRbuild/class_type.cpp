#include "class_type.h"

namespace pache {
// llvm::type

void class_type::set_const() { m_is_const = true; }
void class_type::set_volatile() { m_is_volatile = true; }
} // namespace pache