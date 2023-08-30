#ifndef IR_BUILD_H
#define IR_BUILD_H

#include "../ast/ast.h"
#include "variable.h"
#include "llvm/IR/IRBuilder.h"
#include <memory>
#include <string_view>

namespace pache {
inline namespace IR {
static std::unique_ptr<llvm::LLVMContext> TheContext =
    std::make_unique<llvm::LLVMContext>();
static std::unique_ptr<llvm::Module> TheModule =
    std::make_unique<llvm::Module>("first modlue", *TheContext);
// Create a new builder for the module.
static std::unique_ptr<llvm::IRBuilder<>> Builder =
    std::make_unique<llvm::IRBuilder<>>(*TheContext);
} // namespace IR
class base_build {
public:
  explicit base_build(base_build *father) : m_father(father) {}
  virtual ~base_build() = 0;
  virtual llvm::StructType *find_type(std::string const &name);
  virtual llvm::Value *find_var(std::string const &name) const = 0;
  virtual void insert(std::string const &, build_variable &&value);
  virtual build_type find_type(std::string name) = 0;

protected:
  base_build *m_father;
};

} // namespace pache

#endif
