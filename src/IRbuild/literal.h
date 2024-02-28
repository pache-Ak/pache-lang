#ifndef IR_LITERAL_H
#define IR_LITERAL_H

#include <memory>
#include <unordered_map>
#include <functional>
#include <cstdint>
#include <string_view>
#include "build.h"
#include "variable.h"
#include "ast/literal.h"
namespace pache {


inline namespace integer {
    
template <class Integral>
std::unique_ptr<build_prvalue_variable>
    build_integral(std::intmax_t i);

    std::unordered_map<
        std::string_view, std::function<
            std::unique_ptr<build_variable>(std::intmax_t)
        >> const integer_literal_operators{
    []{
        using namespace std::literals::string_view_literals;

        std::unordered_map<
            std::string_view, std::function<
                std::unique_ptr<build_variable>(
          std::intmax_t)>> val;

        val.emplace("operator \"\"i8"sv, build_integral<i8_type_t>);
        val.emplace("operator \"\"i16"sv, build_integral<i16_type_t>);
        val.emplace("operator \"\"i32"sv, build_integral<i32_type_t>);
        val.emplace("operator \"\"i64"sv, build_integral<i64_type_t>);
       // val.emplace("operator \"\"i128"sv, build_integral<i128_type_t>);
        val.emplace("operator \"\"u8"sv, build_integral<u8_type_t>);
        val.emplace("operator \"\"u16"sv, build_integral<u16_type_t>);
        val.emplace("operator \"\"u32"sv, build_integral<u32_type_t>);
        val.emplace("operator \"\"u64"sv, build_integral<u64_type_t>);
       // val.emplace("operator \"\"u128"sv, build_integral<u128_type_t>);

        return val;
    }()
    };


std::unique_ptr<build_variable>
build_integer_literal(base_build &build, binary_integer_literal const &ast);

std::unique_ptr<build_variable>
build_integer_literal(base_build &build, octal_integer_literal const &ast);

std::unique_ptr<build_variable>
build_integer_literal(base_build &build, decimal_integer_literal const &ast);

std::unique_ptr<build_variable>
build_integer_literal(base_build &build, hexadecimal_integer_literal const &ast);

}



}

#endif
