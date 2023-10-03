#ifndef COMP_UNIT_H
#define COMP_UNIT_H

#include "../ast/compunit.h"
#include "build.h"
#include "class.h"
#include "function.h"
#include "type.h"
#include "variable.h"
#include <functional>
#include <memory>
#include <string>
#include <unordered_map>
#include <unordered_set>
namespace pache {
class file_build : public base_build {
public:
  explicit file_build(compunit_ast *comp);
  /*
      virtual llvm::StructType *find_type(std::string const &name) override {
        auto it = class_def.find(name);
        if (it != class_def.end()) {
          return it->second;
        } else if (m_father != nullptr) {
          return m_father->find_type(name);
        } else {
          return nullptr;
        }
      } */
  virtual std::unique_ptr<build_variable> const &
  find_var(std::string const &name) const override;

private:
  std::unordered_map<std::string, class_build> builded_classes;
  std::unordered_map<std::string, function_build> builded_functions;
  std::unordered_map<std::string, std::reference_wrapper<file_build>>
      import_packges;
  std::unordered_map<std::string, std::unique_ptr<build_variable>>
      global_variables;
};

std::unique_ptr<file_build> build_file(compunit_ast const *const ast);

} // namespace pache

#endif
