#include "function.h"

#include <string>

namespace pache {

std::string_view name_mangling(func_ast const &func) {

  std::string decorated_name = get_father_decorated_name(func.get_father());

  // +
  //                              std::to_string(func->get_name().size()) +
  //                              func->get_name();
  // for (auto &type : func->get_args_type()) {
  //   //  decorated_name += type->decorated_name();
  // }
  return func.get_name();
}
} // namespace pache