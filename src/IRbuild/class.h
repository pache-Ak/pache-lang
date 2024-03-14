#ifndef IR_CLASS_H
#define IR_CLASS_H

#include "../ast/class.h"
#include "../ast/function.h"
#include "../ast/statement.h"
#include "build.h"
#include "class_type.h"
#include "function.h"
#include <memory>
#include <string_view>
#include <unordered_map>
#include <utility>
#include "llvm/IR/DerivedTypes.h"

namespace pache {
class class_build final: public base_build {
public:
  explicit class_build(base_build *const father, std::string &&name, llvm::StructType *type)
    : base_build(father, std::move(name)), m_type(type) {}

  class_type const &get_type() const {return m_type;}

virtual std::set<reference_ptr<function_build>> find_function(std::string_view name) const override {
  if (m_father != nullptr) {
  std::set<reference_ptr<function_build>> s{m_father->find_function(name)};


 // auto [begin, end]{m_functions.equal_range(name)};
 // while (begin != end) {
 //   s.emplace(reference_ptr<function_build>(begin->second));
//  }
  return s;
  } else {
    
  } 
}

virtual reference_ptr<build_variable>
qualified_var_lookup(std::string_view name) override;
virtual reference_ptr<build_type const>
qualified_type_lookup(std::string_view name) const override;
virtual reference_ptr<base_build>
qualified_scope_lookup(std::string_view name) override;

void forward_statement_inner_classes(class_ast const &ast);

void forward_statement_class(class_ast const &inner);

void define_class_body(class_ast const &ast);

auto forward_statement_class_function(func_ast const &ast)
    -> reference_ptr<function_build>;

private:
  std::string m_decorated_name;
  class_type m_type;
  std::unordered_map<std::string_view, std::unique_ptr<class_build>> builded_classes;
  std::unordered_map<
    std::string_view,
    std::unordered_map<reference_ptr<function_type const>, function_build>
  > builded_functions1;
  reference_ptr<class_build> qualified_class_lookup(std::string_view name);
};

class_build forward_statement(base_build &build, class_ast const &ast);
} // namespace pache

#endif
