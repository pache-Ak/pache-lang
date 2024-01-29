#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

#include "ast.h"
#include "statement.h"
#include "type.h"
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pache {

class func_arg : public variable_ast {
public:
  explicit func_arg(type_ast *type, std::string &&str)
      : variable_ast(type, std::move(str)) {}

private:
};

class func_ast : public callable_ast {
public:
  explicit func_ast(std::string &&name,
                    std::pair<std::vector<std::unique_ptr<pache::type_ast>>,
                              std::vector<std::string>> &&args,
                    std::unique_ptr<pache::type_ast> &&return_type,
                    std::unique_ptr<block_ast> &&block)
      : m_name(std::move(name)),
        m_args_name(std::move(args.second)), m_type_ast(std::move(args.first), std::move(return_type)),
        m_block(std::move(block)) {}
  //explicit func_ast() {}

  //void set_name(std::string &&name) { m_name = std::move(name); }

  //std::vector<std::unique_ptr<pache::type_ast>> const &get_args_type() const {
    //return m_args_type;
  //}
  func_type_ast const &get_type() const {
    return m_type_ast;
  }
  std::vector<std::string> const &get_args_name() const { return m_args_name; }
  //type_ast *const get_return_type() const { return m_type.get(); }
  std::string_view get_name() const { return m_name; }
  std::vector<std::pair<std::unique_ptr<type_ast>, std::string>> const&get_args() const{
    return m_args;
  }
  auto const &get_block() const { return m_block; }
  func_type_ast const &get_type() {return m_type_ast; }
protected:
  std::string m_name;
  std::vector<std::pair<std::unique_ptr<type_ast>, std::string>> m_args;
  std::vector<std::string> m_args_name;
  // TODO arguments' type and name have two, only need one.
  func_type_ast const m_type_ast;
  std::unique_ptr<block_ast> m_block;
};

// main func

} // namespace pache

#endif
