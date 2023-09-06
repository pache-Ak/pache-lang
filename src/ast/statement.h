#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"
#include "expression.h"
#include "type.h"

#include <algorithm>
#include <iostream>
#include <memory>
#include <string>
#include <utility>
#include <vector>

namespace pache {
class base_build;
class stmt_ast : public base_ast {
public:
  virtual ~stmt_ast() = default;
  virtual void build(base_build *const father) const = 0;
};

class block_ast : public stmt_ast {
public:
  block_ast() = default;
  block_ast(block_ast &&other) = default;
  block_ast(std::vector<std::unique_ptr<stmt_ast>> &&statements)
      : m_statements(std::move(statements)) {}

  std::vector<std::unique_ptr<stmt_ast>> const &get_stmt_list() const {
    return m_statements;
  }
  virtual void build(base_build *const father) const override;

  // virtual llvm::Value *codegen() override;

  virtual ~block_ast() override = default;

protected:
  std::vector<std::unique_ptr<stmt_ast>> m_statements;
};

class loop_stmt : public block_ast {
public:
  explicit loop_stmt(std::unique_ptr<block_ast> &&block)
      : m_block(std::move(block)) {}
  // virtual llvm::Value *codegen() override;
  //   virtual std::string begin_lable() const override { return begin; }

  //  virtual std::string end_lable() const override { return end; }
  virtual void build(base_build *const father) const override;

private:
  std::unique_ptr<block_ast> m_block;
  // std::string begin;
  // std::string end;
};

class break_stmt : public stmt_ast {
public:
  virtual void build(base_build *const father) const override;
};

class continue_stmt : public stmt_ast {
public:
  virtual void build(base_build *const father) const override;
};

class return_void_stmt : public stmt_ast {
public:
  explicit return_void_stmt() = default;
  virtual void build(base_build *const father) const override;
  // virtual llvm::Value *codegen() override;
  virtual ~return_void_stmt() override = default;
};

class return_ast : public stmt_ast {
public:
  explicit return_ast(std::unique_ptr<exp_ast> &&exp) : m_exp(std::move(exp)) {}

  // virtual llvm::Value *codegen() override;
  virtual ~return_ast() override = default;
  exp_ast const *const get_exp() const { return m_exp.get(); }
  virtual void build(base_build *const father) const override;

private:
  std::unique_ptr<exp_ast> m_exp;
};

class assign_stmt : public stmt_ast {
public:
  explicit assign_stmt(var_exp *var, std::unique_ptr<exp_ast> val)
      : m_var(var), m_val(std::move(val)) {
    m_var->set_father(get_father());
    m_val->set_father(get_father());
  }

  var_exp const *const get_var() const { return m_var; }
  exp_ast const *const get_exp() const { return m_val.get(); }
  virtual void build(base_build *const father) const override;

private:
  var_exp *m_var;
  std::unique_ptr<exp_ast> m_val;
};

class let_stmt : public stmt_ast {
public:
  explicit let_stmt(std::unique_ptr<type_ast> &&type_, std::string &&name,
                    std::unique_ptr<exp_ast> init)
      : type(std::move(type_)), var_name(std::move(name)),
        m_init(std::move(init)) {
    set_father(get_father());
    if (m_init != nullptr) {
      m_init->set_father(get_father());
    }
  }

  virtual void build(base_build *const father) const override;
  /*   explicit let_stmt(variable_ast *var, std::unique_ptr<exp_ast> init)
        : m_var(var), m_init(init) {
      m_var->set_father(get_father());
      if (m_init != nullptr) {
        m_init->set_father(get_father());
      }
    } */

  type_ast const *const get_var_type() const { return type.get(); }
  std::string const &get_var_name() const { return var_name; }
  std::unique_ptr<exp_ast> const &get_init_exp() const { return m_init; }

private:
  std::unique_ptr<type_ast> type;
  std::string const var_name;
  std::unique_ptr<exp_ast> m_init;
};

class exp_stmt : public stmt_ast {
public:
  explicit exp_stmt(std::unique_ptr<exp_ast> &&exp) : m_exp(std::move(exp)) {}
  exp_ast const *const get_exp() const { return m_exp.get(); }
  virtual void build(base_build *const father) const override;

private:
  std::unique_ptr<exp_ast> m_exp;
};

class for_ast : public stmt_ast {

private:
  block_ast *m_block;
  // range    TODO
  // var      TODO
};

class if_stmt : public stmt_ast {
public:
  explicit if_stmt(std::unique_ptr<exp_ast> &&exp,
                   std::unique_ptr<block_ast> &&then)
      : m_condition(std::move(exp)), m_then_block(std::move(then)) {
    m_condition->set_father(get_father());
    m_then_block->set_father(get_father());
  }

  virtual void build(base_build *const father) const override;
  exp_ast const *const get_condition() const { return m_condition.get(); }
  block_ast const *const get_then_block() const { return m_then_block.get(); }

private:
  std::unique_ptr<exp_ast> m_condition;
  std::unique_ptr<block_ast> m_then_block;
};

class if_else_stmt : public stmt_ast {
public:
  explicit if_else_stmt(std::unique_ptr<exp_ast> &&exp,
                        std::unique_ptr<block_ast> &&then, block_ast *elses)
      : m_condition(std::move(exp)), m_then_block(std::move(then)),
        m_else_block(std::move(elses)) {
    m_condition->set_father(get_father());
    m_then_block->set_father(get_father());
    if (m_else_block != nullptr) {
      m_else_block->set_father(get_father());
    }
  }

  virtual void build(base_build *const father) const override;

  exp_ast const *const get_condition() const { return m_condition.get(); }
  block_ast const *const get_then_block() const { return m_then_block.get(); }
  block_ast const *const get_else_block() const { return m_else_block.get(); }

private:
  std::unique_ptr<exp_ast> m_condition;
  std::unique_ptr<block_ast> m_then_block;
  std::unique_ptr<block_ast> m_else_block;
};

} // namespace pache

#endif
