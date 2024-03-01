#ifndef TYPE_H
#define TYPE_H

#include "ast.h"
#include "scope.h"
#include <cstddef>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace pache {
/* enum class value_category {
  prvalue,
  xvalue,
  lvalue,
}; */
class build_type;
class base_build;
class type_ast : public base_ast {
public:
  virtual ~type_ast() = 0;
  virtual std::unique_ptr<build_type> build(base_build &father) const = 0;

  explicit type_ast() = default;
protected:
  type_ast(type_ast const &other) = default;
  type_ast &operator=(type_ast const &other) = default;
};

class mut_ast final: public type_ast {
public:
  mut_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}

  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  type_ast const & get_element_type() const { return *m_type; }
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_type;
};

class volatile_ast final: public type_ast {
public:
  volatile_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}

  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  type_ast const & get_element_type() const { return *m_type; }
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_type;
};

class reference_ast final: public type_ast {
public:
  reference_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  type_ast const & get_element_type() const { return *m_type; }
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_type;
};

class pointer_ast final: public type_ast {
public:
  pointer_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  type_ast const & get_element_type() const { return *m_type; }
  virtual void print() const override;

private:
  std::unique_ptr<type_ast> m_type;
};

// class arr_ast : public type_ast {
// public:
//   explicit arr_ast(std::unique_ptr<type_ast> &&element_type,
//                            std::vector<std::size_t> &&size)
//       : m_element_type(std::move(element_type)), m_size(std::move(size)) {}
//   virtual std::unique_ptr<build_type> build(base_build &father) const override;
//   std::unique_ptr<type_ast> const & get_element_type() const { return m_element_type; }
//   std::vector<std::size_t> const& get_size() const {
//     return m_size;
//   }
// private:
//   std::unique_ptr<type_ast> m_element_type;
//   std::vector<std::size_t> const m_size;
// };

class func_type_ast final : public type_ast {
public:
  explicit func_type_ast(std::vector<std::unique_ptr<type_ast>> &&args,
                     std::unique_ptr<type_ast> &&return_type);
  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  type_ast const & get_return_type() const { return *m_return_type; }
  std::vector<std::unique_ptr<type_ast>> const &get_args_type() const;
  virtual void print() const override;

private:
  std::vector<std::unique_ptr<type_ast>> m_args_type;
  std::unique_ptr<type_ast> m_return_type;
};

class named_type_ast final : public type_ast  {
public:
  explicit named_type_ast(named_ast &&ast) : m_iden(std::move(ast)) {}
  explicit named_type_ast(std::unique_ptr<scope_ast> &&scope, std::string&& identifier)
      :m_iden(std::move(scope), std::move(identifier)) {}
  virtual std::unique_ptr<build_type> build(base_build &father) const override;
  std::string_view get_name() const;
    scope_ast const &get_father_scope() const {
    return m_iden.get_father_scope();
  }
  virtual void print() const override;

private:
  named_ast m_iden;
};

/* class void_type_ast final : public type_ast {
public:
  static std::unique_ptr<type_ast> get();
  virtual std::unique_ptr<build_type> build(base_build &father) const override;

private:
}; */
} // namespace pache

#endif
