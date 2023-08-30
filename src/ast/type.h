#ifndef TYPE_H
#define TYPE_H

#include "ast.h"
#include <cstddef>
#include <memory>
#include <string>
#include <vector>

#include "llvm/IR/Value.h"

using namespace std::literals;

namespace pache {
/* enum class value_category {
  prvalue,
  xvalue,
  lvalue,
}; */
class build_type;
class type_ast : public base_ast {
public:
  // virtual llvm::Type *codegen() = 0;

  // virtual bool is_integral() const { return false; }

  // virtual bool is_signed() const { return false; }

  // virtual bool is_unsigned() const { return false; }

  // bool is_const() const { return m_is_const; }

  // virtual std::string const decorated_name() const = 0;
  virtual ~type_ast() = default;
  // virtual std::string location() { return get_father()->location(); }
  virtual std::unique_ptr<build_type> build() = 0;

protected:
  explicit type_ast() = default;
  explicit type_ast(type_ast const &type) = default;
  explicit type_ast(type_ast &&type) = default;
  // bool m_is_const = false;
};

class const_type : public type_ast {
public:
  const_type(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  //  virtual bool is_const() const { return true; }
  std::string log_error();
  // if type is const type
  // TODO　log error
  // like this
  // file name: in ...
  // ...
  // file name : line : error :duplicate const
  // error line

  virtual std::unique_ptr<build_type> build() override;

private:
  std::unique_ptr<type_ast> m_type;
};

class volatile_type {
public:
  volatile_type(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  // virtual bool is_volatile() const { return true; }

private:
  std::unique_ptr<type_ast> m_type;
};

class reference_ast : type_ast {
public:
  reference_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}
  // bool is_reference() const { return true; }

private:
  std::unique_ptr<type_ast> m_type;
};

class pointer_ast : public type_ast {
public:
  pointer_ast(std::unique_ptr<type_ast> &&type) : m_type(std::move(type)) {}

private:
  std::unique_ptr<type_ast> m_type;
};

class arr_ast : public type_ast {
public:
  explicit arr_ast(std::unique_ptr<type_ast> &&element_type,
                   const std::size_t size)
      : m_element_type(std::move(element_type)), m_size(size) {}
  // virtual llvm::Type *codegen() override;

  // std::unique_ptr<type_ast> get() { return std::unique_ptr<arr_ast>(this); }
  // virtual std::string const decorated_name() const override {
  //   return "_TODO_arr"s;
  //}

private:
  std::unique_ptr<type_ast> m_element_type;
  const std::size_t m_size;
};

class multi_array_ast : public type_ast {
public:
  explicit multi_array_ast(std::unique_ptr<type_ast> &&element_type,
                           std::vector<std::size_t> &&size)
      : m_element_type(std::move(element_type)), m_size(std::move(size)) {}
  // virtual llvm::Type *codegen() override { return nullptr; }
  // virtual std::string const decorated_name() const override {
  //   return "_TODO_mulri_arr"s;
  //  }

private:
  std::unique_ptr<type_ast> m_element_type;
  std::vector<std::size_t> const m_size;
};

class variable_ast;
class func_type : public type_ast {
public:
  explicit func_type(std::vector<std::unique_ptr<type_ast>> &&args,
                     std::unique_ptr<type_ast> &&return_type);

  // std::unique_ptr<type_ast> get() { return std::unique_ptr<func_type>(this);
  // }
  //  virtual std::string const decorated_name() const override {
  //   return "_TODO_func"s;
  //  }

private:
  std::vector<std::unique_ptr<type_ast>> m_args_type;
  std::unique_ptr<type_ast> m_return_type;
};
class class_ast;

class compunit_ast;

class user_def_type : public type_ast {
public:
  explicit user_def_type(std::string &&identifier)
      : m_iden(std::move(identifier)) {}

  std::unique_ptr<type_ast> get();
  // virtual llvm::Type *codegen() override;
  virtual ~user_def_type() = default;
  // virtual std::string const decorated_name() const override {
  //   return "_TODO_class"s;
  // }

private:
  std::string m_iden;
};
} // namespace pache

#endif
