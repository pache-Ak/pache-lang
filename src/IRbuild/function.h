#ifndef IR_FUNCTION_H
#define IR_FUNCTION_H

#include "../ast/function.h"
#include "../ast/type.h"
#include "build.h"
#include "function_type.h"
#include "statement.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Verifier.h"
#include <memory>
#include <stack>
#include <string_view>
#include <utility>
#include <vector>
using namespace std::literals;

namespace pache {
std::string_view name_mangling(func_ast const &func);
std::string_view name_mangling(function_type const & func_type);

class function_build final : public base_build {
public:
  function_build(const function_build &) = default;
  function_build(function_build &&) = default;
  function_build &operator=(const function_build &) = default;
  function_build &operator=(function_build &&) = default;
  explicit function_build(base_build *const father, func_ast const &func);
  explicit function_build(base_build *father, std::string_view name,
                          function_type &&type,
                          llvm::Function *llvm_function,
                          std::vector<
                            std::pair<std::string_view,
                                      std::unique_ptr<build_variable>>> && args)
      : base_build(father), decorated_name(name), m_type(std::move(type)),
        m_llvm_function(llvm_function), var(std::move(args)) {}

  // function_build(function_build &&other) = default;

  virtual std::unique_ptr<build_variable>
  find_var(std::string_view name) const override;

  virtual std::unique_ptr<build_type> find_type(std::string_view name) const override;
  virtual llvm::Function *get_llvm_function() const { return m_llvm_function; }
  function_type const &get_function_type() const {
    return m_type;
  }

private:
  std::string_view decorated_name;
  function_type m_type;
  llvm::Function *m_llvm_function;
  std::vector<
    std::pair<std::string_view,
    std::unique_ptr<build_variable>
  >> var;
};
} // namespace pache

#endif
