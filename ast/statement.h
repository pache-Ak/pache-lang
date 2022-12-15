#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"
#include "expression.h"

#include <vector>

namespace pache {
  class stmt_ast : public base_ast {
  public:
    virtual ~stmt_ast() override { }
  };

  class block_ast : public stmt_ast {
  public:
    block_ast() = default;
    block_ast(block_ast && other) = default;
    block_ast(std::vector<stmt_ast*> &&statements) : m_statements(std::move(statements)) { }
    virtual void insert_dec(std::string name, variable_ast *dec) override {
      dec_name.insert(std::make_pair(name, dec));
    }

    virtual variable_ast * find_dec(const std::string& name) const override {
      auto beg = this->dec_name.find(name);
        if (beg != this->dec_name.end()) {
          return beg->second;
        } else if (m_father != nullptr) {
          return m_father->find_dec(name);
        } else {
          return nullptr;
        }
    }





    virtual ~block_ast() override = default;
  protected:
    std::vector<stmt_ast*> m_statements;

    std::unordered_map<std::string, variable_ast*> dec_name;
  };

  class loop_stmt : public block_ast {
  public:
    explicit loop_stmt(block_ast *block) : m_block(block) { }


    virtual std::string begin_lable() const override {
      return begin;
    }

    virtual std::string end_lable() const  override {
      return end;
    }

  private:
    block_ast *m_block;
    std::string begin;
    std::string end;
  };

  class break_stmt : public stmt_ast {
  public:



  };

  class continue_stmt : public stmt_ast {
  public:



  };

  class return_ast : public stmt_ast {
  public:
    explicit return_ast(exp_ast* exp) : m_exp(exp) { }

    virtual ~return_ast() override = default;


  private:
    exp_ast* m_exp;
  };


  class assign_stmt : public stmt_ast {
  public:
    explicit assign_stmt(var_exp *var, exp_ast *val)
      : m_var(var), m_val(val) {
      m_var->set_father(m_father);
      m_val->set_father(m_father);
    }


  private:
    var_exp *m_var;
    exp_ast *m_val;
  };

  class let_stmt : public stmt_ast {
  public:
    explicit let_stmt(variable_ast *var, exp_ast * init)
      : m_var(var), m_init(init) {
        m_var->set_father(this);
        if (m_init != nullptr) {
          m_init->set_father(this);
        }
      }



  private:
    std::unique_ptr<variable_ast> m_var;
    std::unique_ptr<exp_ast> m_init;
  };

  class exp_stmt : public stmt_ast {
  public:
    explicit exp_stmt(exp_ast *exp) : m_exp(exp) { }


  private:
    exp_ast *m_exp;
  };

  class for_ast : public stmt_ast {


  private:
    block_ast m_block;
    // range    TODO
    // var      TODO
  };

  class if_stmt : public block_ast {
  public:
    explicit if_stmt(exp_ast *exp, block_ast *then, block_ast *elses)
      : m_condition(exp), m_then_block(then), m_else_block(elses) {
        m_condition->set_father(m_father);
        m_then_block->set_father(m_father);
        if (m_else_block != nullptr) {
          m_else_block->set_father(m_father);
        }
      }

    virtual variable_ast * find_dec(const std::string &name) const override {
      return m_father->find_dec(name);
    }

  private:
    exp_ast *m_condition;
    block_ast *m_then_block;
    block_ast *m_else_block;
  };

}

#endif
