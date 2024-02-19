#include "ast.h"

namespace pache {
base_ast::~base_ast(){}
void base_ast::print() const {}
}

//pache::base_ast *const pache::base_ast::get_father() const { return m_father; }

/* std::string pache::get_father_decorated_name(pache::base_ast *ast) {
  if (ast == nullptr) {
    return ""s;
  }
  return get_father_decorated_name(ast->get_father()) + ast->decorated_name();
} */

//void pache::base_ast::set_father(base_ast *father) { m_father = father; }