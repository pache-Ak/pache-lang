#include "ast.h"
#include "statement.h"
#include "llvm/IR/Verifier.h"

pache::base_ast *pache::base_ast::get_father() { return m_father; }

/* std::string pache::get_father_decorated_name(pache::base_ast *ast) {
  if (ast == nullptr) {
    return ""s;
  }
  return get_father_decorated_name(ast->get_father()) + ast->decorated_name();
} */

void pache::base_ast::set_father(base_ast *father) { m_father = father; }