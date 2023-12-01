#include "literal.h"
#include "llvm/IR/Constants.h"
#include <cstdint>
#include <string_view>
#include <limits>
#include "build.h"

namespace pache {
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
std::intmax_t strvtoint(std::string_view sv, std::intmax_t base) {
    // sv must in range of base
    intmax_t i{0};
    for (auto c : sv) {
        i += chartoi(c);
        i *= base;
    }
    return i;
}


std::unique_ptr<build_constant_variable> build_i8(std::intmax_t i) {
    if (i > std::numeric_limits<int8_t>::max()) {
        // TODO log error out of range
        return nullptr;
    }
    return std::make_unique<build_constant_variable>(
        std::make_unique<i8_type_t>(),
        llvm::ConstantInt::get(Builder->getInt8Ty(), i));
}


} // namespace pache
