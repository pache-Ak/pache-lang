#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"
#include "expression.h"

#include <memory>
#include <string_view>
#include <vector>

namespace pache {
class block_scope;
class stmt_ast : public base_ast {
public:
  explicit stmt_ast() = default;
  virtual ~stmt_ast() = 0;
  virtual void build(block_scope &father) const = 0;

protected:
  stmt_ast(stmt_ast const &other) = default;
  stmt_ast &operator=(stmt_ast const &other) = default;
  stmt_ast(stmt_ast &&other) = default;
  stmt_ast&operator=(stmt_ast &&other) = default;
};

class block_ast : public stmt_ast {
public:
  explicit block_ast(std::vector<std::unique_ptr<stmt_ast>> &&statements);

  std::vector<std::unique_ptr<stmt_ast>> const &get_stmt_list() const {
    return m_statements;
  }
  virtual void build(block_scope &father) const override;

protected:
  block_ast(block_ast &&other) = default;
  block_ast(block_ast const &other) = default;
  std::vector<std::unique_ptr<stmt_ast>>  m_statements;
};

class loop_stmt final : public block_ast {
public:
  explicit loop_stmt(block_ast &&block)
      : block_ast(std::move(block)) {}

  virtual void build(block_scope &father) const override;

private:
  
};

class break_stmt final : public stmt_ast {
public:
  virtual void build(block_scope &father) const override;
};

class continue_stmt final : public stmt_ast {
public:
  virtual void build(block_scope &father) const override;
};

class return_void_stmt final : public stmt_ast {
public:
  virtual void build(block_scope &father) const override;
};

class return_ast final : public stmt_ast {
public:
  explicit return_ast(std::unique_ptr<exp_ast> &&exp) : m_exp(std::move(exp)) {}

  exp_ast const *const get_exp() const { return m_exp.get(); }
  virtual void build(block_scope &father) const override;

private:
  std::unique_ptr<exp_ast> m_exp;
};

class assign_stmt final : public stmt_ast {
public:
  explicit assign_stmt(std::unique_ptr<exp_ast> &&var,
                       std::unique_ptr<exp_ast> &&val)
      : m_var(std::move(var)), m_val(std::move(val)) {
    m_var->set_father(get_father());
    m_val->set_father(get_father());
  }

  exp_ast const &get_var() const { return *m_var; }
  exp_ast const &get_exp() const { return *m_val; }
  virtual void build(block_scope &father) const override;

private:
  std::unique_ptr<exp_ast> m_var;
  std::unique_ptr<exp_ast> m_val;
};

class let_stmt final : public stmt_ast {
public:
  explicit let_stmt(std::unique_ptr<type_ast> &&type_, std::string_view name,
                    std::unique_ptr<exp_ast> init)
      : type(std::move(type_)), var_name(std::move(name)),
        m_init(std::move(init)) {
    set_father(get_father());
    if (m_init != nullptr) {
      m_init->set_father(get_father());
    }
  }

  virtual void build(block_scope &father) const override;
  /*   explicit let_stmt(variable_ast *var, std::unique_ptr<exp_ast> init)
        : m_var(var), m_init(init) {
      m_var->set_father(get_father());
      if (m_init != nullptr) {
        m_init->set_father(get_father());
      }
    } */

  type_ast const &get_var_type() const { return *type; }
  std::string_view get_var_name() const { return var_name; }
  std::unique_ptr<exp_ast> const &get_init_exp() const { return m_init; }

private:
  std::unique_ptr<type_ast> type;
  std::string_view var_name;
  std::unique_ptr<exp_ast> m_init;
};

class exp_stmt final : public stmt_ast {
public:
  explicit exp_stmt(std::unique_ptr<exp_ast> &&exp) : m_exp(std::move(exp)) {}
  exp_ast const &get_exp() const { return *m_exp; }
  virtual void build(block_scope &father) const override;

private:
  std::unique_ptr<exp_ast> m_exp;
};

// TODO
// class for_ast final : public stmt_ast {

// private:
//   block_ast *m_block;
//   // range    TODO
//   // var      TODO
// };

class if_stmt final : public stmt_ast {
public:
  explicit if_stmt(std::unique_ptr<exp_ast> &&exp,
                   std::unique_ptr<block_ast> &&then)
      : m_condition(std::move(exp)), m_then_block(std::move(then)) {
    m_condition->set_father(get_father());
    m_then_block->set_father(get_father());
  }

  virtual void build(block_scope &father) const override;
  exp_ast const & get_condition() const { return *m_condition; }
  block_ast const & get_then_block() const { return *m_then_block; }

private:
  std::unique_ptr<exp_ast> m_condition;
  std::unique_ptr<block_ast> m_then_block;
};

class if_else_stmt final : public stmt_ast {
public:
  explicit if_else_stmt(std::unique_ptr<exp_ast> &&exp,
                        std::unique_ptr<block_ast> &&then,
                        std::unique_ptr<stmt_ast> &&elses)
      : m_condition(std::move(exp)), m_then_block(std::move(then)),
        m_else_block(std::move(elses)) {
    m_condition->set_father(get_father());
    m_then_block->set_father(get_father());
    if (m_else_block != nullptr) {
      m_else_block->set_father(get_father());
    }
  }

  virtual void build(block_scope &father) const override;

  exp_ast const & get_condition() const { return *m_condition; }
  block_ast const & get_then_block() const { return *m_then_block; }
  stmt_ast const & get_else_block() const { return *m_else_block; }

private:
  std::unique_ptr<exp_ast> m_condition;
  std::unique_ptr<block_ast> m_then_block;
  std::unique_ptr<stmt_ast> m_else_block;
};

} // namespace pache

#endif
