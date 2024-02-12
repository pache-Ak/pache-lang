#include "scope.h"

namespace pache {
    std::string_view named_ast::get_name() const { return m_iden; }
//void qualified_scope_ast::append(std::string_view name) { m_names.push_back(name); }
} // namespace pache