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
                            std::string_view name,
                           llvm::Function *llvm_function)
     : build_variable(std::move(type)), decorated_name(name), 
      m_llvm_function(llvm_function){}

  // function_build(function_build &&other) = default;
  virtual llvm::Value *get_value() const override{ return m_llvm_function; }
  virtual ~function_build() = default;
  virtual std::unique_ptr<build_variable> clone() const override;
  function_type const & get_type() const { return static_cast<function_type const &>(*m_type); }
  //virtual llvm::Function *get_llvm_function() const { return m_llvm_function; }
 //// function_type const &get_function_type() const {
 //   return m_type;
 // }

private:
 // std::unique_ptr<build_type> find_type(std::string_view name) const ;
 // std::unique_ptr<build_variable>
//find_var(std::string_view name) const ;
  std::string_view decorated_name;
 // function_type m_type;
  llvm::Function *m_llvm_function;
 // std::vector<
 //   std::pair<std::string_view,
 //   std::unique_ptr<build_variable>
 // >> var;
};
} // namespace pache

#endif
