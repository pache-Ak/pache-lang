#include "class.h"
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
