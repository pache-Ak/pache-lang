#include "literal.h"

llvm::Value *pache::void_literal_t::codegen() {
    return nullptr;
}

llvm::Value *pache::i32_literal::codegen() {
    return llvm::ConstantInt::get(Builder->getInt32Ty(), m_value);
}

