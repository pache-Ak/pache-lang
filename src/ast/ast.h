#ifndef AST_H
#define AST_H

namespace pache {
class base_ast {
public:
  explicit base_ast() = default;
  virtual ~base_ast() = 0;
  virtual void print() const = 0;
protected:
  base_ast &operator=(base_ast const &other) = default;
  base_ast(base_ast const &other) = default;
};

} // namespace pache

#endif
