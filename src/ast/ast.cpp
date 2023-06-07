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

void pache::base_ast::set_father(pache::base_ast *scope) { m_father = scope; }
pache::base_ast *pache::base_ast::get_father() { return m_father; }

/* llvm::AllocaInst *
pache::base_ast::find_named_Value(std::string const &name) const {
  return nullptr;
}; */

/* void pache::base_ast::insert_named_Value(std::string const &name,
                                         llvm::AllocaInst *val) {} */

llvm::Value *pache::func_ast::codegen() {
  llvm::Function *TheFunction = TheModule->getFunction(m_name);

  if (TheFunction == nullptr) {
    std::vector<llvm::Type *> args;
    for (auto arg : m_args_type) {
      args.push_back(arg->codegen());
    }

    TheFunction = llvm::Function::Create(
        llvm::FunctionType::get(m_type->codegen(), args, false),
        llvm::Function::ExternalLinkage, m_name, TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : TheFunction->args()) {
      Arg.setName(m_args_name[Idx++]);
    }
  }

  if (TheFunction == nullptr) {
    return nullptr;
  }

  // Create a new basic block to start insertion into.
  llvm::BasicBlock *BB =
      llvm::BasicBlock::Create(*TheContext, "entry", TheFunction);
  Builder->SetInsertPoint(BB);

  /*     // Record the function arguments in the NamedValues map.
      NamedValues.clear();
      for (auto &Arg : TheFunction->args())
          NamedValues[std::string(Arg.getName())] = &Arg; */

  if (llvm::Value *RetVal = m_block->codegen()) {
    // Finish off the function.
    Builder->CreateRet(RetVal);

    // Validate the generated code, checking for consistency.
    llvm::verifyFunction(*TheFunction);

    return TheFunction;
  }

  // Error reading body, remove function.
  TheFunction->eraseFromParent();
  return nullptr;
}
