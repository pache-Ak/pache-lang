#ifndef COMP_UNIT_H
#define COMP_UNIT_H

#include "../ast/compunit.h"
#include "build.h"
#include "class.h"
#include "function.h"
#include "variable.h"
#include <functional>
#include <memory>
#include <string_view>
#include <unordered_map>
namespace pache {
class file_build : public base_build {
public:
  explicit file_build(compunit_ast const *const comp);
  virtual std::unique_ptr<build_variable >
  find_var(std::string_view name) const override;
  virtual std::unique_ptr<build_type> find_type(std::string_view name) const override;
  void make_statement_file();

private:
  std::unordered_map<std::string_view, class_build> builded_classes;
  std::unordered_map<std::string_view, function_build> builded_functions;
  std::unordered_map<std::string_view, std::reference_wrapper<file_build>>
      import_packges;
  std::unordered_map<std::string_view, std::unique_ptr<build_variable>>
      global_variables;
};

std::unique_ptr<file_build> build_file(compunit_ast const *const ast);

} // namespace pache

#endif
