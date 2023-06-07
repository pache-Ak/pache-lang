#ifndef COMPUNIT_H
#define COMPUNIT_H

#include "ast.h"
#include "class.h"
#include "expression.h"
#include "statement.h"
#include "type.h"
#include <algorithm>
#include <cstddef>
#include <memory>

namespace pache {
class compunit_ast : public base_ast {
public:
  virtual llvm::Value *codegen() override {
    for (auto &stmt : m_var_table) {

      if (stmt != nullptr) {
        auto val = stmt->codegen();
        if (val) {
          //   val->print(llvm::errs());
        }
        std::cout << "comp unit.\n";
      }
    }
    return nullptr;
  } // TODO
  virtual void insert_dec(stmt_ast *let) {
    let->set_father(get_father());
    m_var_table.push_back(std::unique_ptr<stmt_ast>(let));
  }
  void insert_dec(func_ast *func) {}
  void insert_dec(variable_ast *func) {
    m_var_table.push_back(std::unique_ptr<variable_ast>(func));
    func->set_father(get_father());
  }
  virtual void insert_dec(std::string &&func_name, func_ast *func) {
    m_func_table.emplace(std::move(func_name), func);
    func->set_father(get_father());
  }
  virtual void insert_dec(std::string &&name, variable_ast *dec) {
    dec_name.emplace_back(std::move(name), dec);
  }

  /*   virtual variable_ast *find_dec(const std::string &name) const override {
      auto beg = this->m_symbol_table.find(name);
      if (beg != this->m_symbol_table.end()) {
        return beg->second;
      } else if (get_father() != nullptr) {
        return get_father()->find_dec(name);
      } else {
        return nullptr;
      }
    } */

  void insert_class_def(class_ast *p) {
    p->set_father(get_father());
    class_def.push_back(p);
  }

  /*   class_ast *find_class_def(std::string const &iden) {
      return class_def.find(iden) != class_def.end()
                 ? class_def.find(iden)->second
                 : nullptr;
    } */
  // virtual llvm::AllocaInst *
  // find_named_Value(std::string const &name) const override;
  // virtual void insert_named_Value(std::string const &name,
  //                                 llvm::AllocaInst *val) override;
  virtual func_ast *ADL(std::string const &name,
                        std::vector<exp_ast *> const &args) {
    /*     auto [begin, end] = m_func_table.equal_range(name);
        std::vector<type_ast const *> args_type;
        for (auto exp : args) {
          args_type.emplace_back(exp->get_type());
        }
        for (; begin != end; ++begin) {
          auto beg = args_type.begin();
        loop1:
          if (beg == args_type.end()) {
            goto break1;
          }
          auto [end1, end2] = std::mismatch(args_type.begin(), args_type.end(),
                                            beg->second->) break1:
        } */
    return nullptr; // TODO
  }

private:
  std::vector<std::unique_ptr<base_ast>> m_var_table;
  std::vector<std::pair<std::string, variable_ast *>> dec_name;
  std::unordered_map<std::string, variable_ast *> m_symbol_table;
  std::vector<class_ast *> class_def;
  std::unordered_map<std::string, llvm::AllocaInst *> m_named_values;
  std::unordered_multimap<std::string, std::unique_ptr<callable_ast>>
      m_func_table;
};

} // namespace pache

#endif