#ifndef IMPORT_H
#define IMPORT_H

#include "ast.h"
#include <string_view>
namespace pache {
class import_ast final : public base_ast {
    public:
    std::string_view get_file_name() const {
        return m_file_name;
    }
private:
    std::string_view m_file_name;
};
} // namespace pache

#endif
