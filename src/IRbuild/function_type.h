#ifndef IR_FUNCTION_TYPE_H
#define IR_FUNCTION_TYPE_H

#include "type.h"
#include "llvm/IR/Type.h"
#include <cstddef>
#include <llvm/IR/DerivedTypes.h>
#include <memory>
#include <vector>
using namespace std::literals;
namespace pache {
class function_type : public build_type {
public:
  template <class Iterator>
  explicit function_type(std::unique_ptr<build_type> &&return_type,
                         Iterator begin, Iterator end)
      : m_return_type(std::move(return_type)) {
    while (begin != end) {
      m_args_type.emplace_back(std::move(*begin));
      ++begin;
    }
  }

  explicit function_type(std::unique_ptr<build_type> &&return_type,
                         std::vector<std::unique_ptr<build_type>> &&args_type,
                         llvm::FunctionType *llvm_type)
      : m_return_type(std::move(return_type)),
        m_args_type(std::move(args_type)), m_llvm_type(llvm_type) {}
  virtual llvm::FunctionType *get_llvm_type() const override {
    return m_llvm_type;
  }
  virtual void set_const() override {
    std::cerr << "can't fix function with const.";
  }
  virtual void set_volatile() override {
    std::cerr << "can't fix function with volatile.";
  }
  std::unique_ptr<build_type> const &get_return_type() const {
    return m_return_type;
  }
  std::vector<std::unique_ptr<build_type>> const &get_args_type() const {
    return m_args_type;
  }

private:
  std::unique_ptr<build_type> m_return_type;
  std::vector<std::unique_ptr<build_type>> m_args_type;
  llvm::FunctionType *m_llvm_type;
};
} // namespace pache

#endif
