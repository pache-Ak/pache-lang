#include "type.h"
#include "ast.h"

namespace pache {
  const i32_type_t i32_type_t::i32_type{};
  const void_type_t void_type_t::void_type{};
  const bool_type_t bool_type_t::bool_type{};

  func_type::func_type(const std::vector<variable_ast*> &args, const type_ast* return_type)
    : m_return_type(return_type) {
      std::for_each(args.begin(), args.end(),
                    [=](variable_ast* var){m_args_type.push_back(var->get_type());});
    }
}
