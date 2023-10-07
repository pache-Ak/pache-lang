#ifndef AST_H
#define AST_H

#include "llvm/IR/IRBuilder.h"
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>

namespace pache {
class base_ast {
public:
  virtual ~base_ast() = default;
  void set_father(base_ast *father);
  base_ast *get_father();

  // virtual std::string location() = 0;

protected:
  base_ast *m_father;
  std::size_t m_line_number;
};
std::string get_father_decorated_name(base_ast *ast);

class callable_ast : public base_ast {};
class type_ast;
class variable_ast : public callable_ast {
public:
  explicit variable_ast(type_ast *type, std::string name)
      : m_type(type), real_name(name) {}

  type_ast *get_type() { return m_type; }
  std::string const &get_name() const { return real_name; }

  void set_name(std::string name) { real_name = name; }

protected:
  type_ast *m_type;
  std::string real_name;
};

} // namespace pache

#endif
