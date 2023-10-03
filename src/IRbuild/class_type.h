#ifndef IR_CLASS_TYPE_H
#define IR_CLASS_TYPE_H

#include "class.h"
#include "type.h"
#include "llvm/IR/Type.h"

// using namespace std::literals;
namespace pache {
class class_type : public build_type {
public:
  virtual llvm::StructType *get_llvm_type() const override;
  virtual void set_const() override;
  virtual void set_volatile() override;

private:
  std::unique_ptr<class_build> m_def;
};
} // namespace pache

#endif
