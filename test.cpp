#include <iostream>

struct A {
    A() {
        throw std::runtime_error("copy constructor exception");
    }
    A(A const &other) {
        std::cout << "copy" << std::endl;
    }

    A &operator=(A const &other) {
        std::cout << "copy assignment" << std::endl;
        return *this;
    }
};

A a;
int main() {
    std::cout << "main" << std::endl;
    A b = a;
    A c{b};
}