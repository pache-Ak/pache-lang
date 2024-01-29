#ifndef IR_EXPRESSION_H
#define IR_EXPRESSION_H

#include "../ast/expression.h"
#include "../reference_ptr.h"
#include "build.h"
#include "function.h"
#include "variable.h"
#include <memory>

namespace pache {
std::unique_ptr<build_variable> build_exp(base_build &build,
                                                exp_ast const &ast);
std::unique_ptr<build_variable> build_unary_plus(base_build &build,
                                                       unary_plus const &ast);
std::unique_ptr<build_variable> build_unary_minus(base_build &build,
                                                        unary_minus const &ast);
std::unique_ptr<build_variable>
build_func_call_exp(base_build &build, func_call_exp const &ast);
std::unique_ptr<build_variable> build_var_exp(base_build &build,
                                                    var_exp const &ast);
std::unique_ptr<build_variable>
build_binary_mul_exp(base_build &build, binary_mul_exp const &ast);
std::unique_ptr<build_variable>
build_binary_div_exp(base_build &build, binary_div_exp const &ast);
std::unique_ptr<build_variable>
build_binary_mod_exp(base_build &build, binary_mod_exp const &ast);
std::unique_ptr<build_variable>
build_binary_plus_exp(base_build &build, binary_plus_exp const &ast);
std::unique_ptr<build_variable>
build_binary_minus_exp(base_build &build, binary_minus_exp const &ast);
std::unique_ptr<build_variable>
build_three_way_exp(base_build &build, three_way_exp const &ast);
std::unique_ptr<build_variable> build_less_exp(base_build &build,
                                                     less_exp const &ast);
std::unique_ptr<build_variable> build_less_eq_exp(base_build &build,
                                                        less_eq_exp const &ast);
std::unique_ptr<build_variable> build_greater_exp(base_build &build,
                                                        greater_exp const &ast);
std::unique_ptr<build_variable>
build_greater_eq_exp(base_build &build, greater_eq_exp const &ast);
std::unique_ptr<build_variable> build_eq_exp(base_build &build,
                                                   eq_exp const &ast);
std::unique_ptr<build_variable> build_not_eq_exp(base_build &build,
                                                       not_eq_exp const &ast);
std::unique_ptr<build_variable>
build_bitwise_and_exp(base_build &build, bitwise_and_exp const &ast);
std::unique_ptr<build_variable>
build_bitwise_xor_exp(base_build &build, bitwise_xor_exp const &ast);
std::unique_ptr<build_variable>
build_bitwise_or_exp(base_build &build, bitwise_or_exp const &ast);
std::unique_ptr<build_variable>
build_logical_and_exp(base_build &build, logical_and_exp const &ast);
std::unique_ptr<build_variable>
build_logical_or_exp(base_build &build, logical_or_exp const &ast);

std::unique_ptr<build_variable>
build_subscript_exp(base_build &build, subscript_exp const &ast);

// find function 
template <class Iterator>
reference_ptr<function_build>
ADL(base_build const &build, std::string_view function_name, Iterator begin,
    Iterator end);

template <class Iterator>
reference_ptr<function_build>
function_lookup(base_build const &build, std::string_view name, Iterator begin,
                Iterator end) {
  //std::vector<reference_ptr<function_build>>;
   //   set = ADL(build, "operator+"sv, begin, end);

  ///  TODO

  return nullptr;
}

} // namespace pache

#endif
