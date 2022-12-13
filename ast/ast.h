#ifndef AST_H
#define AST_H

// #include "llvm/ADT/APFloat.h"
// #include "llvm/ADT/STLExtras.h"
// #include "llvm/IR/BasicBlock.h"
// #include "llvm/IR/Constants.h"
// #include "llvm/IR/DerivedTypes.h"
// #include "llvm/IR/Function.h"
// #include "llvm/IR/IRBuilder.h"
// #include "llvm/IR/LLVMContext.h"
// #include "llvm/IR/Module.h"
// #include "llvm/IR/Type.h"
// #include "llvm/IR/Verifier.h"
#include <algorithm>
#include <memory>
#include <vector>
#include <string>
#include <string_view>
#include <iostream>
#include <unordered_map>
#include "type.h"

namespace pache {
  inline namespace {
    std::size_t ssa_value = 0;
  }

  class variable_ast;
  class func_ast;
  class stmt_ast;
  class base_ast {
  public:
    virtual ~base_ast() = default;
    virtual std::string dump() = 0;
    void set_father(base_ast *father) {
      m_father = father;
      return;
    }
    virtual void insert_dec(variable_ast *dec ) { };
    virtual void insert_dec(std::string name, variable_ast * dec)  {
      m_father->insert_dec(name, dec);
    }
    virtual void insert_dec(stmt_ast *let) { }
    virtual variable_ast * find_dec(const std::string& name) const {
      if (m_father != nullptr)
      return m_father->find_dec(name);

      return nullptr;
    }
    void insert_class_def(class_ast *p) {

    }
    virtual std::string begin_lable() const {
      if (m_father != nullptr) {
        return m_father->begin_lable();
      } else {
        std::cout << "error : not in loop block.\n";
        return "";
      }
    }

    virtual std::string end_lable() const {
      if (m_father != nullptr) {
        return m_father->end_lable();
      } else {
        std::cout << "error : not in loop block.\n";
        return "";
      }
    }
    std::string get_prefix() {
      return name_prefix;
    }

    void set_prefix(std::string pre) {
      name_prefix = pre;
    }

    std::size_t next_block_value() {
      return block_value++;
    }
  protected:
    base_ast *m_father = nullptr;
    std::unordered_map<std::string, type_ast*> type_name;
    std::string name_prefix;
    std::size_t block_value = 0;
  };
  class variable_ast : public base_ast {
  public:
    explicit variable_ast(const type_ast *type, std::string name)
      : m_type(type), real_name(name) { }

    const type_ast * get_type() {
      return m_type;
    }

    std::string &get_name() {
      return real_name;
    }

    void set_name(std::string name) {
      real_name = name;
    }
    virtual std::string dump() override {
      return "";
    }
  protected:
    const type_ast *m_type;
    std::string real_name;
  };







// ...
    class func_arg : public variable_ast {
  public:
    explicit func_arg(const type_ast *type, std::string &&str) : variable_ast(type, std::move(str)) { }
    virtual std::string dump() {
      return get_type()->dump() + "%" + get_name();
    }

  private:
  };


  class func_ast : public variable_ast {
  public:
    explicit
    func_ast(std::string &&name,
             std::vector<func_arg*> args,
             const type_ast* return_type,
             base_ast *block)
      : variable_ast(return_type, std::move(name)),
        m_args(args),
        m_block(block) { }

    virtual variable_ast* find_dec(const std::string &name) const override {
      auto beg = std::find_if(m_args.begin(), m_args.end(),
                              [=](func_arg* arg) { return (arg->get_name() == name); });
      if (beg != m_args.end()) {
        return *beg;
      } else {
        return m_father->find_dec(name);
      }
    }
    virtual std::string dump() override {
      std::cout << "define "
          << get_type()->dump()
          << " @" << real_name << "(";
      for (auto arg : m_args) {
        std::cout << arg->get_type()->dump()
                  << " %" << arg->get_name()
                  << ", ";
      }
      std::cout    << ") {"
          << m_block->dump()
          << "}\n";

      return std::string{};
    }
  protected:
    std::vector<func_arg*> m_args;
    std::unique_ptr<base_ast> m_block;
  };
    class main_func_ast : public func_ast {
  public:
    explicit
    main_func_ast(std::string &&name,
             std::vector<func_arg*> args,
             const type_ast* return_type,
             base_ast *block)
      : func_ast(std::move(name), args, return_type, block){}

    virtual std::string dump() override {
      std::cout << "define "
          << "i32"
          << " @" << "main" << "("
    //  for (auto arg : m_args) {
      //  std::cout << arg->
      //}
          << ") "
          << m_block->dump();

      return std::string{};
    }


  };

  class class_ast : public base_ast {
  public:
    explicit class_ast(std::string &&name, std::vector<base_ast*> &&class_body)
      : m_name(std::move(name)), m_body(std::move(class_body)) {
        for (auto & p : m_body) {
          p->set_father(this);
        }
      }

    std::string const get_name() const {
      return m_name;
    }

    virtual std::string dump() override {
      return "";
    }
  private:
    std::string const m_name;
    std::vector<base_ast*> m_body;
  };

}


#endif
