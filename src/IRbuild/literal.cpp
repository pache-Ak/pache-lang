#include "literal.h"
#include "build.h"
#include "variable.h"
#include "llvm/IR/Constants.h"
#include <cstdint>
#include <string_view>
#include <limits>

namespace pache {
inline namespace integer {
    int chartoi(char c) {
        switch (c) {
        case '0':
        return 0;
        case '1':
        return 1;
        case '2':
        return 2;
        case '3':
        return 3;
        case '4':
        return 4;
        case '5':
        return 5;
        case '6':
        return 6;
        case '7':
        return 7;
        case '8':
        return 8;
        case '9':
        return 9;
        case 'a':
        case 'A':
        return 10;
        case 'b':
        case 'B':
        return 11;
        case 'c':
        case 'C':
        return 12;
        case 'd':
        case 'D':
        return 13;
        case 'e':
        case 'E':
        return 14;
        case 'f':
        case 'F':
        return 15;
        default:
        return 0;
        }
    }
std::intmax_t strvtoint(std::string_view const sv, std::intmax_t base) {
    // sv must in range of base
    intmax_t i{0};
    for (auto c : sv) {
        i *= base;
        i += chartoi(c);
    }
    return i;
}

template <class Integral>
std::unique_ptr<build_prvalue_variable> build_integral(std::intmax_t i) {
    if (i > std::numeric_limits<typename Integral::c_type>::max()) {
        // TODO log error out of range
        return nullptr;
    }
    return std::make_unique<build_prvalue_variable>(
        std::make_unique<Integral>(),
        llvm::ConstantInt::get(Integral().get_llvm_type(), i));
}

std::function<std::unique_ptr<build_variable> (std::intmax_t)>
  find_integer_literal_operator(base_build &build, std::string_view suffix) {
    std::string s = "operator \"\"" + std::string{suffix};
    if (auto it = integer_literal_operators.find(s); it != integer_literal_operators.end()) {
      return it->second;
    } else {
      //return build.unqualified_lookup(suffix);
    }
  }


std::unique_ptr<build_variable>
build_integer_literal(base_build &build, binary_integer_literal const&ast) {
    std::intmax_t i{strvtoint(ast.m_literal, 2)};
    auto f{find_integer_literal_operator(build, ast.m_suffix)};
    if (f == nullptr) {
        return nullptr;
    }
    return f(i);
}

std::unique_ptr<build_variable>
build_integer_literal(base_build &build, octal_integer_literal const&ast) {
    std::intmax_t i{strvtoint(ast.m_literal, 8)};
    auto f{find_integer_literal_operator(build, ast.m_suffix)};
    if (f == nullptr) {
        return nullptr;
    }
    return f(i);
}


std::unique_ptr<build_variable>
build_integer_literal(base_build &build, decimal_integer_literal const&ast) {
    std::intmax_t i{strvtoint(ast.m_literal, 10)};
    auto f{find_integer_literal_operator(build, ast.m_suffix)};
    if (f == nullptr) {
        return nullptr;
    }
    return f(i);
}


std::unique_ptr<build_variable>
build_integer_literal(base_build &build, hexadecimal_integer_literal const&ast) {
    std::intmax_t i{strvtoint(ast.m_literal, 16)};
    auto f{find_integer_literal_operator(build, ast.m_suffix)};
    if (f == nullptr) {
        return nullptr;
    }
    return f(i);
}

    }
inline namespace boolean {
 
    std::unique_ptr<build_prvalue_variable>
    build_boolean_literal(base_build &build, boolean_literal const &ast) {
      return std::make_unique<build_prvalue_variable>(
          std::make_unique<bool_type_t>(),
          llvm::ConstantInt::get(bool_type_t().get_llvm_type(),
                                 ast.get_literal()));
    }
}
} // namespace pache
