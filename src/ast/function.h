#ifndef AST_FUNCTION_H
#define AST_FUNCTION_H

#include "ast.h"
#include "statement.h"
#include <string>
#include <utility>

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
      : m_name(std::move(name)), m_args_type(std::move(args.first)),
        m_args_name(std::move(args.second)), m_type(std::move(return_type)),
        m_block(std::move(block)) {}
  explicit func_ast() {}
  //  virtual llvm::Value *codegen() override;

  void set_name(std::string &&name) { m_name = std::move(name); }
  /*virtual variable_ast *find_dec(const std::string &name) const override {
    auto beg = std::find_if(m_args.begin(), m_args.end(), [=](func_arg *arg) {
      return (arg->get_name() == name);
    });
    if (beg != m_args.end()) {
      return *beg;
    } else {
      return m_father->find_dec(name);
    }
  }*/
  std::vector<std::unique_ptr<pache::type_ast>> const &get_args_type() const {
    return m_args_type;
  }
  std::vector<std::string> const &get_args_name() const { return m_args_name; }
  type_ast *const get_return_type() const { return m_type.get(); }
  std::string const &get_name() const { return m_name; }
  /*   std::string const get_decorated_name() const {
      std::string s = "_N" + m_father->decorated_name() + get_name();
      for (auto &type : m_args_type) {
        s += type->decorated_name();
      }
      return s;
    } */

protected:
  std::string m_name;
  std::vector<std::unique_ptr<pache::type_ast>> m_args_type;
  std::vector<std::string> m_args_name;
  std::unique_ptr<pache::type_ast> m_type;
  std::unique_ptr<block_ast> m_block;
  /*   std::string name_mangling() {
      std::string s{};
      s += get_father()->decorated_name();
      s += this->m_name;
      for (auto &arg : m_args_type) {
        s += arg->decorated_name();
      }
      return s;
    } */
};
/* class main_func_ast : public func_ast {
public:
  explicit main_func_ast(std::string &&name, std::vector<func_arg *> args,
                         type_ast *return_type, base_ast *block)
      : func_ast(std::move(name), std::move(args), return_type, block) {}
}; */
} // namespace pache

#endif
