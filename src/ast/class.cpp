#include "class.h"
#include "ast.h"
#include <algorithm>

pache::class_ast::class_ast(std::string &&name, class_body &&body)
    : m_name(std::move(name)), m_body(std::move(body)) {
  for (auto &p : m_body.var_def) {
    p->set_father(this);
  }
  for (auto &p : m_body.func_def) {
    p->set_father(get_father());
  }
  for (auto &p : m_body.var_def) {
    p->set_father(this);
  }
}
