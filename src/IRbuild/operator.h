#ifndef IRBUILD_OPERATOR_H
#define IRBUILD_OPERATOR_H

#include "function.h"
#include <vector>

namespace pache {
extern std::vector<function_build> operator_unary_plus;
extern std::vector<function_build> operator_unary_minus;
extern std::vector<function_build> operator_binary_mul;
extern std::vector<function_build> operator_binary_div;

} // namespace pache

#endif
