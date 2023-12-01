#ifndef IR_BUILD_H
#define IR_BUILD_H

#include "llvm/IR/IRBuilder.h"
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
#include "../ast/literal.h"
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
class build_type;
class build_variable;
class base_build {
public:
  explicit base_build(base_build *const father) : m_father(father) {}
  virtual ~base_build() = 0;
  virtual std::unique_ptr<build_variable>
  find_var(std::string_view name) const = 0;
  virtual void insert(std::string &&, std::unique_ptr<build_variable> &&value);
  virtual void insert(std::string_view name,
                      std::unique_ptr<build_variable> &&value);
  virtual std::unique_ptr<build_type> find_type(std::string_view name) const = 0;
  virtual std::function<std::unique_ptr<build_variable>(std::string_view,
                                                        literal)>
  find_integer_literal_operator(std::string_view suffix);
void unqualified_lookup(std::string_view name);
protected:
  base_build(base_build const &other) = default;
 // implicitlt delete
 // base_build &operator=(base_build const &other) = default;
  // point to father scope maybe is nullptr
  base_build *const m_father;
  static std::unordered_map<
    std::string_view, std::unique_ptr<build_type>> const
      prprimary_type;
  static std::unordered_map<
    std::string_view, std::function<
        std::unique_ptr<build_variable>(
          std::string_view,
        literal)>> const
          integer_literal_operators;
};

} // namespace pache

#endif
