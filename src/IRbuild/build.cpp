#include "build.h"

namespace pache {
std::unordered_map<std::string_view, std::unique_ptr<build_type>> const
base_build::prprimary_type{
    #include "primary_type.inc"
};
} // namespace pache