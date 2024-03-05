#ifndef VARIABLE_H
#define VARIABLE_H

#include "type.h"
#include "llvm/IR/Argument.h"
#include "llvm/IR/Instructions.h"
#include "llvm/IR/Value.h"
#include "llvm/IR/GlobalVariable.h"
#include <memory>

namespace pache {
// every value related a type,
// so vaiable type has a build_type
// build_type is a polymorphic objects
// so type is pointer
class build_variable {
public:
  build_variable(const build_variable &) = delete;
  build_variable(build_variable &&) = delete;
  build_variable &operator=(const build_variable &) = delete;
  build_variable &operator=(build_variable &&) = delete;
  explicit build_variable(std::unique_ptr<build_type const> &&type)
      : m_type(std::move(type)) {}

  build_type const & get_type() const { return *m_type; }
  virtual llvm::Value *get_value() const = 0;
  virtual llvm::AllocaInst* get_address() const { return nullptr; }
  virtual ~build_variable() = 0;
  virtual std::unique_ptr<build_variable> clone() const = 0;

protected:
// implicit delete
  //build_variable(build_variable const &other) = default;
  //build_variable &operator=(build_variable const &other) = default;

  std::unique_ptr<build_type const> const m_type;
private:
};
llvm::Value *get_value(std::unique_ptr<build_variable> const &var);

class build_lvalue : public build_variable {};

class build_local_variable final : public build_variable {
public:
  explicit build_local_variable(std::unique_ptr<build_type const> &&type,
                                llvm::AllocaInst *value)
      : build_variable(std::move(type)), m_value(value) {}

  llvm::Value *get_value() const override {
    return Builder->CreateLoad(m_value->getAllocatedType(), m_value); }
  virtual std::unique_ptr<build_variable> clone() const override;
  virtual llvm::AllocaInst *get_address() const override { return m_value; }
private:
  llvm::AllocaInst *m_value;
};

class build_local_reference final : public build_variable {
public:
  explicit build_local_reference(std::unique_ptr<build_type const> &&type,
                                 llvm::AllocaInst *value)
      : build_variable(std::move(type)), m_value(value) {}

  virtual llvm::LoadInst *get_value() const override;
  virtual std::unique_ptr<build_variable> clone() const override;

private:
  llvm::AllocaInst *m_value;
};

class build_global_variable final : public build_variable {
public:
  explicit build_global_variable(std::unique_ptr<build_type const> &&type,
                                 llvm::GlobalVariable *value)
      : build_variable(std::move(type)), m_value(value) {}

  virtual llvm::Constant  *get_value() const override { return m_value->getInitializer(); }
  virtual std::unique_ptr<build_variable> clone() const override;

private:
  llvm::GlobalVariable *m_value;
};

// Constant
// can be evaluated at compile time.
class build_constant_variable final : public build_variable {
public:
  explicit build_constant_variable(std::unique_ptr<build_type const> &&type,
                                   llvm::Constant *value)
      : build_variable(std::move(type)), m_value(value) {}

  llvm::Constant *get_value() const override { return m_value; }
  virtual std::unique_ptr<build_variable> clone() const override;

private:
  llvm::Constant *m_value;
};

class build_prvalue_variable final : public build_variable {
public:
  explicit build_prvalue_variable(std::unique_ptr<build_type const> &&type,
                                  llvm::Value *value)
      : build_variable(std::move(type)), m_value(value) {}

  llvm::Value *get_value() const override { return m_value; }
  virtual std::unique_ptr<build_variable> clone() const override;

private:
  llvm::Value *m_value;
};

class build_argument_variable final : public build_variable {
public:
  explicit build_argument_variable(std::unique_ptr<build_type const> &&type,
                                   llvm::Argument *value)
    : build_variable(std::move(type)), m_value(value) {}

  llvm::Value *get_value() const override { return m_value; }
  virtual std::unique_ptr<build_variable> clone() const override;

private:
  llvm::Argument *m_value;
};

} // namespace pache

#endif
