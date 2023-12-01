#include "scope.h"

namespace pache {
void scope_ast::append(std::string_view name) { m_names.push_back(name); }
} // namespace pache