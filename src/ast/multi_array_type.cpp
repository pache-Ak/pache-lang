#include "multi_array_type.h"
#include "IRbuild/type.h"

#include <iostream>

namespace pache {
std::unique_ptr<build_type> multi_array_ast::build(base_build &father) const {
  return build_multi_array_type(father, *this);
}


void multi_array_ast::print() const {
  std::cout << "multi_array_ast:\n"
            << "m_element_type:\n";
  m_element_type->print();
  std::cout << "m_size:{\n";
  for (auto const &s : m_size) {
    s->print();
  }
  std::cout << "}\n";
}
}