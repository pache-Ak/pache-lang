#ifndef IR_CLASS_H
#define IR_CLASS_H

#include "../ast/class.h"
#include "../ast/function.h"
#include "../ast/statement.h"
#include "build.h"
#include "function.h"
#include <memory>
#include <string_view>
#include <unordered_map>

namespace pache {
class class_build : public base_build {
public:
  class data_member {
  public:
    explicit data_member(std::unique_ptr<build_type> const &type,
                         std::size_t i);
    std::unique_ptr<build_type> const &get_type() const;
    std::size_t const get_num() const;

  private:
    std::unique_ptr<build_type> const &m_type;
    std::size_t const m_num;
  };
  explicit class_build(
      base_build *const father,
      /* std::string_view  &name,  */ llvm::StructType *llvm_type);
  explicit class_build(base_build *const father,
                       std::unique_ptr<class_ast> const &ast);

  llvm::StructType *get_IRtype() const { return m_type; }
  virtual std::unique_ptr<build_variable >
  find_var(std::string_view name) const override;
  llvm::Value *get_value(llvm::Value *class_value) const {}
  virtual std::unique_ptr<build_type> find_type(std::string_view name) const override;

private:
  llvm::Value *get_member_var(llvm::Value *ptr, std::string_view name);
  std::unordered_multimap<std::string_view, function_build> m_functions;
  std::unordered_map<std::string_view, data_member> m_member_var;
  // std::string_view  m_name; // used by log error
  llvm::StructType *m_type;
  std::unordered_map<std::string_view, std::unique_ptr<class_build>>
      builded_classes;
};
} // namespace pache

#endif
