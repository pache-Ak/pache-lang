#include "ast.h"
#include "statement.h"
#include "llvm/IR/Verifier.h"

static std::unique_ptr<llvm::LLVMContext> TheContext =
    std::make_unique<llvm::LLVMContext>();
static std::unique_ptr<llvm::Module> TheModule =
    std::make_unique<llvm::Module>("first modlue", *TheContext);
// Create a new builder for the module.
static std::unique_ptr<llvm::IRBuilder<>> Builder =
    std::make_unique<llvm::IRBuilder<>>(*TheContext);

// void pache::base_ast::set_father(pache::base_ast *scope) { m_father = scope;
// }
pache::base_ast *pache::base_ast::get_father() { return m_father; }

/* std::string pache::get_father_decorated_name(pache::base_ast *ast) {
  if (ast == nullptr) {
    return ""s;
  }
  return get_father_decorated_name(ast->get_father()) + ast->decorated_name();
} */

/* llvm::AllocaInst *
pache::base_ast::find_named_Value(std::string const &name) const {
  return nullptr;
}; */

/* void pache::base_ast::insert_named_Value(std::string const &name,
                                         llvm::AllocaInst *val) {} */

void pache::base_ast::set_father(base_ast *father) { m_father = father; }