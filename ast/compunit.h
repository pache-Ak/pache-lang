#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "statement.h"
#include <cstddef>

namespace pache {
  class compunit_ast : public base_ast {
  public:

    virtual void insert_dec(stmt_ast *let) override {
      let->set_father(this);
      m_var_table.push_back(std::unique_ptr<stmt_ast>(let));
    }

    virtual void insert_dec(variable_ast *func) override {
      m_var_table.push_back(std::unique_ptr<variable_ast>(func));
      func->set_father(this);
    }

    virtual void insert_dec(std::string name, variable_ast *dec) override {
      dec_name.insert(std::make_pair(name, dec));
    }

    virtual variable_ast * find_dec(const std::string &name) const override {
      auto beg = this->dec_name.find(name);
        if (beg != this->dec_name.end()) {
          return beg->second;
        } else if (m_father != nullptr) {
          return m_father->find_dec(name);
        } else {
          return nullptr;
        }
    }

    void insert_class_def(class_ast *p) {
      p->set_father(this);
      class_def.insert(std::make_pair(p->get_name(), p));
    }

    class_ast *find_class_def(std::string const & iden) {
      return class_def.find(iden) != class_def.end() ?
             class_def.find(iden)->second : nullptr;
    }
  private:
    std::vector<std::unique_ptr<base_ast>> m_var_table;
    std::unordered_map<std::string, variable_ast*> dec_name;
    std::unordered_map<std::string, class_ast*> class_def;
  };

}

#endif