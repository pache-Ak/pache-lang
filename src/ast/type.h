#ifndef TYPE_H
#define TYPE_H

#include "ast.h"
#include <memory>
#include <string>
#include <vector>
using namespace std::literals;

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
  virtual std::unique_ptr<build_type> build(base_build *const father) const = 0;

  explicit type_ast() = default;
};

class const_type : public type_ast {
public:
  const_type(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}

  virtual std::unique_ptr<build_type>
  build(base_build *const father) const override;
  type_ast const *const get_element_type() const { return m_type.get(); }

private:
  std::unique_ptr<type_ast> m_type;
};

class volatile_type : public type_ast {
public:
  volatile_type(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}

  virtual std::unique_ptr<build_type>
  build(base_build *const father) const override;
  type_ast const *const get_element_type() const { return m_type.get(); }

private:
  std::unique_ptr<type_ast> m_type;
};

class reference_ast : type_ast {
public:
  reference_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  virtual std::unique_ptr<build_type>
  build(base_build *const father) const override;
  type_ast const *const get_element_type() const { return m_type.get(); }

private:
  std::unique_ptr<type_ast> m_type;
};

class pointer_ast : public type_ast {
public:
  pointer_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  virtual std::unique_ptr<build_type>
  build(base_build *const father) const override;

private:
  std::unique_ptr<type_ast> m_type;
};

class arr_ast : public type_ast {
public:
  explicit arr_ast(std::unique_ptr<type_ast> &&element_type,
                   const std::size_t size)
      : m_element_type(std::move(element_type)), m_size(size) {}

private:
  std::unique_ptr<type_ast> m_element_type;
  const std::size_t m_size;
};

class multi_array_ast : public type_ast {
public:
  explicit multi_array_ast(std::unique_ptr<type_ast> &&element_type,
                           std::vector<std::size_t> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}

private:
  std::unique_ptr<type_ast> m_element_type;
  std::vector<std::size_t> const m_size;
};

class variable_ast;
class func_type : public type_ast {
public:
  explicit func_type(std::vector<std::unique_ptr<type_ast>> &&args,
                     std::unique_ptr<type_ast> &&return_type);

  virtual ~func_type() = default;

private:
  std::vector<std::unique_ptr<type_ast>> m_args_type;
  std::unique_ptr<type_ast> m_return_type;
};

class user_def_type : public type_ast {
public:
  explicit user_def_type(std::string &&identifier)
      : m_iden(std::move(identifier)) {}

  std::unique_ptr<type_ast> get();
  virtual ~user_def_type() = default;

private:
  std::string m_iden;
};

class void_type_ast : public type_ast {
public:
  static std::unique_ptr<type_ast> get();
  virtual std::unique_ptr<build_type>
  build(base_build *const father) const override;

private:
};
} // namespace pache

#endif
