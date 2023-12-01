#include "class_type.h"
#include "ast/type.h"
#include <memory>
#include <iostream>

namespace pache {
// llvm::type

void class_type::set_mutable() {   if (!is_const()) {
    std::cerr << "redef mutable.\n";
  } else {
    m_is_const = false;
  } }
void class_type::set_volatile() {   if (is_volatile()) {
    std::cerr << "redef volatile.\n";
  } else {
    m_is_volatile = true;
  } }
std::unique_ptr<build_type> class_type::clone() const {
  return std::unique_ptr<class_type>(new class_type{*this});
}
llvm::StructType *class_type::get_llvm_type() const {
  return m_ref.get_IRtype();
}
class_type::class_type(class_build const &ref) : m_ref(ref) {}
} // namespace pache