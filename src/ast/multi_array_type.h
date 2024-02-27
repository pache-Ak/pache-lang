#ifndef MULTI_ARRAY_TYPR_H
#define MULTI_ARRAY_TYPE_H

#include "type.h"
#include "expression.h"

namespace pache {
class multi_array_ast final: public type_ast {
public:
  explicit multi_array_ast(std::unique_ptr<type_ast> &&element_type,
                           std::vector<std::unique_ptr<exp_ast>> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}
  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  std::unique_ptr<type_ast> const & get_element_type() const { return m_element_type; }
  std::vector<std::unique_ptr<exp_ast>> const& get_size() const {
    return m_size;
  }
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_element_type;
  std::vector<std::unique_ptr<exp_ast>> const m_size;
};

class arr_type;

std::unique_ptr<arr_type>
build_multi_array_type(base_build &father, multi_array_ast const &ast);

}

#endif
