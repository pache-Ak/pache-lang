#ifndef IR_FUNCTION_TYPE_H
#define IR_FUNCTION_TYPE_H

#include "build.h"
#include "type.h"
#include <memory>
#include <vector>

namespace pache {
class function_type final : public build_type {
public:
  function_type(const function_type &) = delete;
  function_type(function_type &&) = default;
  function_type &operator=(const function_type &) = delete;
  function_type &operator=(function_type &&) = default;
  explicit function_type(base_build &build, func_type_ast const &ast);
  explicit function_type(std::unique_ptr<build_type> &&return_type,
                         std::vector<std::unique_ptr<build_type>> &&args_type,
                         llvm::FunctionType *llvm_type);
  explicit function_type(std::unique_ptr<build_type> &&return_type,
                         std::vector<std::unique_ptr<build_type>> &&args_type);
  virtual llvm::FunctionType *get_llvm_type() const override {
    return m_llvm_type;
  }
  virtual void set_mutable() override;
  virtual void set_volatile() override;
  std::unique_ptr<build_type> const get_return_type() const {
    return m_return_type->clone();
  }
  std::vector<std::unique_ptr<build_type>> const &get_args_type() const {
    return m_args_type;
  }
  virtual std::unique_ptr<build_type> clone() const override;

private:
  std::unique_ptr<build_type> m_return_type;
  std::vector<std::unique_ptr<build_type>> m_args_type;
  llvm::FunctionType *m_llvm_type;
};

std::unique_ptr<build_type>
build_func_type(base_build &father, func_type_ast const &ast);
} // namespace pache

#endif
