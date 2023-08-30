#include "../IRbuild/statement.h"
#include "statement.h"

/* llvm::Value *pache::block_ast::codegen() {
  for (auto stmt : m_statements) {
    stmt->codegen();
  }

  // TODO deallocate all variable;

  return nullptr;
} */

void pache::block_ast::build(base_build *father) { build_block(father, this); }

/* llvm::Value *pache::loop_stmt::codegen() {
  llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

  llvm::BasicBlock *loopBB =
      llvm::BasicBlock::Create(*TheContext, "loop", TheFunction);
  llvm::BasicBlock *endEntryBB =
      llvm::BasicBlock::Create(*TheContext, "endEntry", TheFunction);
  llvm::BasicBlock *endLoopBB =
      llvm::BasicBlock::Create(*TheContext, "endLoop", TheFunction);
} */

void pache::loop_stmt::build(base_build *father) { build_loop(father, this); }

llvm::Value *pache::if_stmt::codegen() {
  llvm::Value *cond_V = m_condition->codegen();
  llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*TheContext, "then", TheFunction);
  llvm::BasicBlock *ElseBB =
      llvm::BasicBlock::Create(*TheContext, "else", TheFunction);
  llvm::BasicBlock *MergeBB =
      llvm::BasicBlock::Create(*TheContext, "ifcont", TheFunction);

  Builder->CreateCondBr(cond_V, ThenBB, ElseBB);

  // Emit then value.
  Builder->SetInsertPoint(ThenBB);

  m_then_block->codegen();

  Builder->CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  // ThenBB = Builder->GetInsertBlock();

  // Emit else block.
  // TheFunction->insert(TheFunction->end(), ElseBB);
  Builder->SetInsertPoint(ElseBB);

  m_else_block->codegen();

  Builder->CreateBr(MergeBB);
  // codegen of 'Else' can change the current block, update ElseBB for the PHI.
  // ElseBB = Builder->GetInsertBlock();

  // Emit merge block.
  // TheFunction->insert(TheFunction->end(), MergeBB);
  Builder->SetInsertPoint(MergeBB);
  // llvm::PHINode *PN =
  //   Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext), 2, "iftmp");

  // PN->addIncoming(ThenV, ThenBB);
  // PN->addIncoming(ElseV, ElseBB);
  // return PN;
  return nullptr;
}

void pache::if_else_stmt::build(base_build *father) {
  llvm::Value *cond_V = m_condition->codegen();
  llvm::Function *TheFunction = Builder->GetInsertBlock()->getParent();

  // Create blocks for the then and else cases.  Insert the 'then' block at the
  // end of the function.
  llvm::BasicBlock *ThenBB =
      llvm::BasicBlock::Create(*TheContext, "then", TheFunction);
  llvm::BasicBlock *ElseBB =
      llvm::BasicBlock::Create(*TheContext, "else", TheFunction);
  llvm::BasicBlock *MergeBB =
      llvm::BasicBlock::Create(*TheContext, "ifcont", TheFunction);

  Builder->CreateCondBr(cond_V, ThenBB, ElseBB);

  // Emit then value.
  Builder->SetInsertPoint(ThenBB);

  build_block(father, m_then_block);
  // m_then_block->codegen();

  Builder->CreateBr(MergeBB);
  // Codegen of 'Then' can change the current block, update ThenBB for the PHI.
  // ThenBB = Builder->GetInsertBlock();

  // Emit else block.
  // TheFunction->insert(TheFunction->end(), ElseBB);
  Builder->SetInsertPoint(ElseBB);

  m_else_block->codegen();

  Builder->CreateBr(MergeBB);
  // codegen of 'Else' can change the current block, update ElseBB for the PHI.
  // ElseBB = Builder->GetInsertBlock();

  // Emit merge block.
  // TheFunction->insert(TheFunction->end(), MergeBB);
  Builder->SetInsertPoint(MergeBB);
  // llvm::PHINode *PN =
  //   Builder->CreatePHI(llvm::Type::getDoubleTy(*TheContext), 2, "iftmp");

  // PN->addIncoming(ThenV, ThenBB);
  // PN->addIncoming(ElseV, ElseBB);
  // return PN;
  return nullptr;
}
llvm::Value *pache::return_void_stmt::codegen() { Builder->CreateRetVoid(); }

llvm::Value *pache::return_ast::codegen() {
  Builder->CreateRet(m_exp->codegen());
}