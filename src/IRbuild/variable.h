#ifndef VARIABLE_H
#define VARIABLE_H

#include "type.h"
#include "llvm/IR/IRBuilder.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/LLVMContext.h"
#include "llvm/IR/Module.h"
#include <memory>

namespace pache {
class build_type;

class build_variable {
public:
  explicit build_variable(std::unique_ptr<build_type> const &type)
      : m_type(type) {}

  build_type *get_type() const { return m_type.get(); }
  virtual llvm::Value *get_value() const = 0;
  virtual ~build_variable() = 0;

private:
  std::unique_ptr<build_type> const &m_type;
};

class build_local_variable : public build_variable {
public:
  explicit build_local_variable(std::unique_ptr<build_type> const &type,
                                llvm::AllocaInst *value)
      : build_variable(type), m_value(value) {}

  llvm::AllocaInst *get_value() const override { return m_value; }
  virtual ~build_local_variable() = default;

private:
  llvm::AllocaInst *m_value;
};

class build_global_variable : public build_variable {
public:
  explicit build_global_variable(std::unique_ptr<build_type> const &type,
                                 llvm::GlobalVariable *value)
      : build_variable(type), m_value(value) {}

  virtual llvm::GlobalVariable *get_value() const override { return m_value; }
  virtual ~build_global_variable() = default;

private:
  llvm::GlobalVariable *m_value;
};

// Constant
class build_constant_variable : public build_variable {
public:
  explicit build_constant_variable(std::unique_ptr<build_type> const &type,
                                   llvm::Constant *value)
      : build_variable(type), m_value(value) {}

  llvm::Constant *get_value() const override { return m_value; }
  virtual ~build_constant_variable() = default;

private:
  llvm::Constant *m_value;
};

class build_prvalue_variable : public build_variable {
public:
  explicit build_prvalue_variable(std::unique_ptr<build_type> const &type,
                                  llvm::Value *value)
      : build_variable(type), m_value(value) {}

  llvm::Value *get_value() const override { return m_value; }
  virtual ~build_prvalue_variable() = default;

private:
  llvm::Value *m_value;
};

} // namespace pache

#endif
