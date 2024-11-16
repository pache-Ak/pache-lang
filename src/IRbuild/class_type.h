#ifndef IR_CLASS_TYPE_H
#define IR_CLASS_TYPE_H

#include "IRbuild/variable.h"
#include "build.h"
#include "ast/class.h"
#include "type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <llvm-17/llvm/IR/Value.h>
#include <memory>
#include <string_view>

namespace pache {
class let_stmt;
class class_type final : public build_type {
public:
  class data_member {
  public:
    data_member(const data_member &other) : m_type(other.m_type->clone()), m_num(other.m_num){}
    data_member(data_member &&) = default;
    data_member &operator=(const data_member &) = delete;
    data_member &operator=(data_member &&) = delete;
    explicit data_member(std::unique_ptr<build_type> &&type, std::size_t i);
    build_type const &get_type() const;
    std::size_t const get_num() const;

  private:
    std::unique_ptr<build_type> m_type;
    std::size_t const m_num;
  };
  explicit class_type(llvm::StructType *const type)
    :m_type(type) {}
  explicit class_type() = default;
  class_type(const class_type &) = default;
  class_type(class_type &&) = default;
  class_type &operator=(const class_type &) = default;
  class_type &operator=(class_type &&) = default;
  virtual llvm::StructType *get_llvm_type() const override;
  virtual std::unique_ptr<build_type> clone() const override;

  std::unique_ptr<build_variable>
  get_member_var(llvm::Value *obj, std::string_view name) const;

  void define_body(base_build &build, class_ast const &ast);

private:
  std::unordered_map<std::string_view, data_member> m_member_var;
  llvm::StructType *m_type;

  std::pair<std::unordered_map<std::string_view, data_member>::iterator, bool>
  define_data_members(base_build &build, let_stmt const &ast, std::size_t i);
};
} // namespace pache

#endif
