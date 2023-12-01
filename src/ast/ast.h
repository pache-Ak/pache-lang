#ifndef AST_H
#define AST_H

#include <cstddef>
#include <string>

namespace pache {
class base_ast {
public:
  explicit base_ast() = default;
  virtual ~base_ast() = 0;
  void set_father(base_ast *father);
  base_ast *const get_father() const;

  // virtual std::string location() = 0;

protected:
  base_ast &operator=(base_ast const &other) = default;
  base_ast(base_ast const &other) = default;

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
