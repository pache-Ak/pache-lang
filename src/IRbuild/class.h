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

namespace pache {
class class_build : public base_build {
public:
 // explicit class_build(
 //     base_build *const father,
  //    /* std::string_view  &name,  */ llvm::StructType *llvm_type);
  explicit class_build(base_build *const father,
                       class_ast const &ast);

  class_type const &get_type() const {return m_type;}
  //llvm::StructType *get_IRtype() const { return m_type.get_llvm_type(); }
  virtual reference_ptr<build_variable >
  find_var(std::string_view name) const override;
  virtual std::unique_ptr<build_type> find_type(std::string_view name) const override;
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
  
private:
  std::unordered_multimap<std::string_view, std::unique_ptr<function_build>> m_functions;
  
  // std::string_view  m_name; // used by log error
  class_type m_type;
  std::unordered_map<std::string_view, std::unique_ptr<class_build>>
      builded_classes;
};
} // namespace pache

#endif
