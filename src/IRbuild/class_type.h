#ifndef IR_CLASS_TYPE_H
#define IR_CLASS_TYPE_H

#include "class.h"
#include "type.h"
#include "llvm/IR/Type.h"

namespace pache {
class class_type final : public build_type {
public:
  explicit class_type(class_build const &ref);
  virtual llvm::StructType *get_llvm_type() const override;
  virtual void set_mutable() override;
  virtual void set_volatile() override;
  virtual std::unique_ptr<build_type> clone() const override;

private:
  class_build const &m_ref;
};
} // namespace pache

#endif
