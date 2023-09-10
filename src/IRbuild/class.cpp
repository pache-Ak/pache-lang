#include "class.h"
#include "variable.h"
#include "llvm/IR/Verifier.h"

void pache::class_build::create_member_functions(
    std::vector<std::unique_ptr<func_ast>> &v_func) {
  for (auto &func : v_func) {
    std::vector<llvm::Type *> args_Type;
    for (auto &args : func->get_args_type()) {
      args_Type.push_back(args->codegen());
    }
    llvm::FunctionType *FT = llvm::FunctionType::get(
        func->get_return_type()->codegen(), args_Type, false);
    llvm::Function *F =
        llvm::Function::Create(FT, llvm::Function::ExternalLinkage,
                               func->get_name(), IR::TheModule.get());

    unsigned Idx = 0;
    for (auto &Arg : F->args()) {
      Arg.setName(func->get_args_name()[Idx]);
    }

    llvm::BasicBlock *BB =
        llvm::BasicBlock::Create(*IR::TheContext, "entry", F);
    IR::Builder->SetInsertPoint(BB);

    verifyFunction(*F);
  }
}

// even in member function, class's variable still need assgin by explict arg
// this.
//
// so excpet static variable there is no variable in class.
// static variable will define in namespace.
// don't need find variable in class.
pache::build_variable *const
pache::class_build::find_var(std::string const &name) const {
  std::cerr << "shouldn't access this function!\n";
  return nullptr;
}

llvm::Value *pache::class_build::get_member_var(llvm::Value *ptr,
                                                std::string const &name) {
  auto it = m_member_var.find(name);
  if (it != m_member_var.end()) {
    return IR::Builder->CreateGEP(
        m_type, ptr,
        {IR::Builder->getInt32(0), IR::Builder->getInt32(m_member_var[name])},
        name);
  } else {
    return find_var(name)->get_value();
  }
}
