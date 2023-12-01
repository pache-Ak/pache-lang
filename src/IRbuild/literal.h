#ifndef LITERAL_H
#define LITERAL_H

#include <memory>
#include <string_view>

#include "variable.h"

namespace pache {
inline namespace integer {
std::unique_ptr<build_constant_variable> build_i8(std::string_view sv);
}
}

#endif
