#ifndef IR_CLASS_TYPE_H
#define IR_CLASS_TYPE_H

#include "build.h"
#include "ast/class.h"
#include "type.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Type.h"
#include <string_view>

namespace pache {
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
  explicit class_type(base_build &build, class_ast const&ast);
  //explicit class_type(class_build const &ref);
  explicit class_type() = default;
  class_type(const class_type &) = default;
  class_type(class_type &&) = default;
  class_type &operator=(const class_type &) = default;
  class_type &operator=(class_type &&) = default;
  // explicit class_type(llvm::StructType *type) :m_type(type){}
  virtual llvm::StructType *get_llvm_type() const override;
  virtual void set_mutable() override;
  virtual void set_volatile() override;
  virtual std::unique_ptr<build_type> clone() const override;
  virtual bool is_struct() const override { return true;}
  data_member const * const get_member_var(std::string_view name) const {
    if (auto it = m_member_var.find(name); it != m_member_var.end()) {
      return &it->second;
    } else {
      return nullptr;
    }
  }
  llvm::Value *get_member_var(llvm::Value *ptr, std::string_view name);
private:
  //class_build const &m_ref;
  std::unordered_map<std::string_view, data_member> m_member_var;
  llvm::StructType *m_type;
};
} // namespace pache

#endif
