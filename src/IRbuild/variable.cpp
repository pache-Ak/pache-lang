#include "variable.h"
#include "build.h"
#include <memory>

namespace pache {
build_variable::~build_variable(){}
    
llvm::LoadInst *build_local_reference::get_value() const {
  return Builder->CreateLoad(get_type().get_llvm_type(), m_value);
}
std::unique_ptr<build_variable> build_local_variable::clone() const {
 // return std::unique_ptr<build_local_variable>(
    //  new build_local_variable(get_type(), m_value));
  return std::make_unique<build_local_variable>(get_type().clone(), m_value);
}
std::unique_ptr<build_variable> build_local_reference::clone() const {
 // return std::unique_ptr<build_local_reference>(
 //     new build_local_reference(get_type(), m_value));
      return std::make_unique<build_local_reference>(get_type().clone(), m_value);
}
std::unique_ptr<build_variable> build_global_variable::clone() const {
 // return std::unique_ptr<build_global_variable>(
//new build_global_variable(get_type(), m_value));
      return std::make_unique<build_global_variable>(get_type().clone(), m_value);
}
std::unique_ptr<build_variable> build_constant_variable::clone() const {
//  return std::unique_ptr<build_constant_variable>(
 //     new build_constant_variable(get_type(), m_value));
      return std::make_unique<build_constant_variable>(get_type().clone(), m_value);
}
std::unique_ptr<build_variable> build_prvalue_variable::clone() const {
 // return std::unique_ptr<build_prvalue_variable>(
 //     new build_prvalue_variable(get_type(), m_value));
      return std::make_unique<build_prvalue_variable>(get_type().clone(), m_value);
}

std::unique_ptr<build_variable> build_argument_variable::clone() const {
 // return std::unique_ptr<build_argument_variable>(
 //     new build_argument_variable(get_type(), m_value));
      return std::make_unique<build_argument_variable>(get_type().clone(), m_value);
}
llvm::Value *get_value(std::unique_ptr<build_variable> const &var) {
  return var->get_value();
}
} // namespace pache