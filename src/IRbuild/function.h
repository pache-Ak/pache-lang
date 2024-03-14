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
#include <llvm-17/llvm/IR/Function.h>
#include <memory>
#include <stack>
#include <string_view>
#include <utility>
#include <vector>
#include <optional>

namespace pache {
std::string_view name_mangling(func_ast const &func);
std::string_view name_mangling(function_type const & func_type);

class function_build final : public build_variable {
public:
  function_build(const function_build &) = delete;
  function_build(function_build &&) = delete;
  function_build &operator=(const function_build &) = delete;
  function_build &operator=(function_build &&) = delete;
  explicit function_build(base_build *const father, func_ast const &func);
   explicit function_build(std::unique_ptr<build_type const> &&type,
                            std::string && name,
                           llvm::Function *llvm_function)
     : build_variable(std::move(type)), decorated_name(std::move(name)), 
      m_llvm_function(llvm_function){}
  virtual llvm::Value *get_value() const override{ return m_llvm_function; }
  virtual ~function_build() = default;
  virtual std::unique_ptr<build_variable> clone() const override;
  function_type const & get_type() const { return static_cast<function_type const &>(*m_type); }

  friend bool operator==(function_build const &lhs, function_build const &rhs) {
    return lhs.decorated_name == rhs.decorated_name;
  }

  friend std::hash<function_build>;
  void define(base_build *const father, func_ast const &ast);

private:
  std::string decorated_name;
  llvm::Function *m_llvm_function;
};

std::optional<function_build> forward_statement(base_build &build, func_ast const &ast);
} // namespace pache

namespace std {
    template <>
    struct hash<pache::function_build> {
        std::size_t operator()(const pache::function_build& funcDef) const {
            std::hash<std::string> stringHasher;
            return stringHasher(funcDef.decorated_name);
        }
    };
}

#endif
