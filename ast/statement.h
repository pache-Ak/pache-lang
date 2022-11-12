#ifndef STATEMENT_H
#define STATEMENT_H

#include "ast.h"
#include "expression.h"

#include <vector>

namespace pache {
  class stmt_ast : public base_ast {
  public:
    virtual std::string dump() const override = 0;
    virtual ~stmt_ast() override { }
  };

  class block_ast : public stmt_ast {
  public:
    block_ast(std::vector<stmt_ast*> *statements) : m_statements(statements) { }
    virtual std::string dump() const override {
      std::cout << "{\n";
      for (auto stmt : *m_statements) {
        // std::cout << "\t";
        std::cout << stmt->dump();
      }
      std::cout << "}\n";
      return std::string{};
    }


    virtual ~block_ast() override = default;
  protected:
    std::vector<stmt_ast*> *m_statements;
  };

  class return_ast : public stmt_ast {
  public:
    explicit return_ast(exp_ast* exp) : m_exp(exp) { }
    virtual std::string dump() const override {
      m_exp->set_father(m_father);
      std::string s1 = m_exp->dump();
      std::cout << "ret i32 " << s1 << "\n";
      return std::string{};
    }
    virtual ~return_ast() override = default;


  private:
    exp_ast* m_exp;
  };

  class let_stmt : public stmt_ast {
  public:
    explicit let_stmt(variable_ast *var) : m_var(var) { }

    virtual std::string dump() const override {
      std::string name = m_var->get_name();
      m_var->set_name(name + m_father->get_prefix());
      insert_dec(name, m_var);
      std::cout << "@" << m_var->get_name() << " = alloc " << m_var->get_type()->dump() << "\n";
      return "";
    }

  private:
    variable_ast *m_var;
  };

  class for_ast : public stmt_ast {


  private:
    block_ast m_block;
    // range    TODO
    // var      TODO
  };

  class loop_ast : public stmt_ast {


  private:
    block_ast m_block;
  };
}



#endif
