#ifndef IR_CLASS_H
#define IR_CLASS_H

#include "../ast/class.h"
#include "../ast/function.h"
#include "../ast/statement.h"
#include "build.h"
#include "function.h"
#include <memory>
#include <memory_resource>
#include <string>
#include <unordered_map>

namespace pache {
class class_build : public base_build {
public:
  class data_member {
  public:
    explicit data_member(std::unique_ptr<build_type> const &type, std::size_t i)
        : m_type(type), m_num(i) {}
    std::unique_ptr<build_type> const &get_type() const { return m_type; }
    std::size_t const get_num() const { return m_num; }

  private:
    std::unique_ptr<build_type> const &m_type;
    std::size_t const m_num;
  };
  explicit class_build(
      base_build *father,
      /* std::string const &name,  */ llvm::StructType *llvm_type)
      : base_build(father), /* m_name(name),  */ m_type(llvm_type) {}
  explicit class_build(base_build *const father,
                       std::unique_ptr<class_ast> const &ast);

  llvm::StructType *get_IRtype() const { return m_type; }
  virtual std::unique_ptr<build_variable> const &
  find_var(std::string const &name) const override;
  // llvm::Value *get_value(llvm::Value *class_value) const {}
  virtual build_type *const find_type(std::string const &name) const override;
  virtual ~class_build() = default;

private:
  llvm::Value *get_member_var(llvm::Value *ptr, std::string const &name);
  std::unordered_multimap<std::string, function_build> m_functions;
  std::unordered_map<std::string, data_member> m_member_var;
  // std::string const m_name; // used by log error
  llvm::StructType *m_type;
  std::unordered_map<std::string, std::unique_ptr<class_build>> builded_classes;
};
} // namespace pache

#endif
