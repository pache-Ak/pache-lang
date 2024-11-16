#include "expression.h"
#include "ast/expression.h"
#include "build.h"
#include "class_type.h"
#include "function.h"
#include "function_type.h"
#include "reference_ptr.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <cstddef>
#include <memory>
#include <string_view>
#include <utility>
#include <vector>
#include "scope.h"
#include <iostream>
#include "operator.h"

using namespace std::literals;

namespace pache {
std::unique_ptr<build_variable> build_expression(base_build &build,
                                                       exp_ast const &ast) {
  return ast.build(build);
}

std::tuple<llvm::Value *, llvm::Value *, std::unique_ptr<build_type>>
usual_arithmetic_conversions(build_variable const &lhs, build_variable const &rhs) {
  llvm::Value *vl = lhs.get_value();
  llvm::Value *vr = rhs.get_value();

  build_variable const &bigger{lhs.get_type().get_type_ID() >= rhs.get_type().get_type_ID() ? lhs : rhs};
  std::unique_ptr<build_type> id{bigger.get_type().clone()};
  if (lhs.get_type().is_floating_point() && rhs.get_type().is_floating_point()) {
    if (lhs.get_type() == rhs.get_type()) {
    } else if (lhs.get_type().get_type_ID() > rhs.get_type().get_type_ID()){
      vr = Builder->CreateFPExt(vr, lhs.get_type().get_llvm_type());
    } else {
      vl = Builder->CreateFPExt(vl, rhs.get_type().get_llvm_type());  
    }
  } else if (lhs.get_type().is_signed() && rhs.get_type().is_signed()) {
    if (lhs.get_type() == rhs.get_type()) {
    } else if (lhs.get_type().get_type_ID() > rhs.get_type().get_type_ID()){
      vr = Builder->CreateSExt(vr, lhs.get_type().get_llvm_type());
    } else {
      vl = Builder->CreateSExt(vl, rhs.get_type().get_llvm_type());     
    }
  } else if (lhs.get_type().is_unsigned() && rhs.get_type().is_unsigned()) {
    if (lhs.get_type() == rhs.get_type()) {
    } else if (lhs.get_type().get_type_ID() > rhs.get_type().get_type_ID()){
      vr = Builder->CreateZExt(vr, lhs.get_type().get_llvm_type());
    } else {
      vl = Builder->CreateZExt(vl, rhs.get_type().get_llvm_type());     
    }
  } else {
    std::cout << "vaild exp, both lhs and rhs must be FP or same signed int.\n";
    vl = nullptr;
    vr = nullptr;
  }

  return {vl, vr, std::move(id)};
}


// size shoule be same
// 1 ptr type is unique build_variable
// 2 ptr type is unique build_type
template<class InputIt1, class InputIt2>
bool is_args_type_match(InputIt1 begin, InputIt1 end, InputIt2 begin2) {
    while (begin != end) {
        if ((*begin)->get_type() != *(*begin2)) {
            return false;
        }
        ++begin;
        ++begin2;
    }
    return true;
}

template <class it>
reference_ptr<function_build>
function_lookup(base_build const &build, std::string_view name, it begin, it end) {
  return nullptr;
}

std::unique_ptr<build_variable> 
build_unary_plus(base_build &build,
                 unary_plus const &ast) {
  std::array<std::unique_ptr<build_variable>, 1> arges{
      build_expression(build, ast.get_arg())};
std::cout << static_cast<int>(arges[0]->get_type().get_type_ID()) << "\n";
  if (any_of(arges.begin(), arges.end(),
             [](std::unique_ptr<build_variable> &ptr) -> bool {
               return ptr == nullptr;
             })) {
    // args have problem the error has logged

    return nullptr;


  }

  reference_ptr<function_build> func =
      function_lookup(build, "O1+"sv, arges.begin(), arges.end());

  if (func != nullptr) { 
   std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    function_type const &type{static_cast<function_type const &>(func->get_type())};
    return std::make_unique<build_prvalue_variable>(
        type.get_return_type().clone(),
        Builder->CreateCall(type.get_llvm_type(), func->get_value(), args_Value,
                            "call_O1+"));
  } else if (arges[0]->get_type().is_arithmetic()) {
    return std::make_unique<build_prvalue_variable>(arges[0]->get_type().clone(), arges[0]->get_value());
  } else {
    // TODO log error

    return nullptr;
  }
}

std::unique_ptr<build_variable>
build_unary_minus(base_build &build, unary_minus const &ast) {
  std::array<std::unique_ptr<build_variable>, 1> arges{
      build_expression(build, ast.get_arg())};

  if (any_of(arges.begin(), arges.end(),
             [](std::unique_ptr<build_variable> &ptr) -> bool {
               return ptr == nullptr;
             })) {
    // args have problem the error has logged

    return nullptr;
  }

  reference_ptr<function_build> func =
      function_lookup(build, "O1-"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    function_type const &type{static_cast<function_type const &>(func->get_type())};
    return std::make_unique<build_prvalue_variable>(
        type.get_return_type().clone(),
        Builder->CreateCall(type.get_llvm_type(), func->get_value(), args_Value,
                            "call_O1-"));
  } else if (arges[0]->get_type().is_signed()) {
    return std::make_unique<build_prvalue_variable>(arges[0]->get_type().clone(), 
          Builder->CreateNeg(arges[0]->get_value()));
  } else if (arges[0]->get_type().is_floating_point()) {
    return std::make_unique<build_prvalue_variable>(arges[0]->get_type().clone(), 
          Builder->CreateFNeg(arges[0]->get_value()));
  } else {
    // TODO log error

    return nullptr;
  }
}

std::unique_ptr<build_variable>
build_func_call_exp(base_build &build, func_call_exp const &ast) {
  if (std::unique_ptr<build_variable> var{build_expression(build, ast.get_func())};
    var != nullptr
  ) {
    // function pointer
    if (var->get_type().is_pointer() &&
        static_cast<pointer_type const&>(var->get_type()).get_element_type().is_function()    
    ) {
        std::vector<std::unique_ptr<build_type>> const &args_type{
            static_cast<function_type  &>(
                static_cast<pointer_type const&>(
                    var->get_type()).get_element_type()).get_args_type() 
        };
        std::vector<std::unique_ptr<build_variable>> args;
        for (auto const &arg : ast.get_args()) {
            args.emplace_back(build_expression(build, *arg));
        }
        if ( args_type.size() == args.size() &&
            is_args_type_match(args.begin(), args.end(), args_type.begin())
        ) {
            return std::make_unique<function_build>(static_cast<pointer_type const&>(
                    var->get_type()).get_element_type().clone(), "",
                    llvm::dyn_cast<llvm::Function>(Builder->CreateLoad(static_cast<pointer_type const&>(
                    var->get_type()).get_element_type().get_llvm_type(),
                     var->get_value(), "")));         
        } else {
            // TODO log error
        }
    } //else if (var->get_type().is_function_object()) {
    
    //} 
    else {
        // TODO log error
        // var is not a callable 
    }


  } 
  
  
  //if (auto f{function_lookup(build, std::string_view name, Iterator begin, Iterator end)}){

  //}  

 // reference_ptr<function_build> func =
   //   function_lookup(build, "func_call"sv, args.begin(), args.end());

 // if (func != nullptr) {
   // using namespace std::placeholders;
  //std::vector<std::unique_ptr<build_variable>> args;
 // std::transform(ast.get_args().begin(), ast.get_args().end(),
   //              std::back_inserter(args),
     //            std::bind(build_expression, std::ref(build), 
       //                    std::bind([](std::unique_ptr<exp_ast> const &ptr)->exp_ast const &{return *ptr;}, _1)));

  //  std::vector<llvm::Value *> args_Value;
  //  std::transform(args.begin(), args.end(), std::back_inserter(args_Value), get_value);

   // if (func->get_type().get_return_type()->is_reference()) {
    //  return std::make_unique<build_local_reference>(
     //   func->get_type().get_return_type().clone(),
      //  Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value, ""));
   // } else {
    //  return std::make_unique<build_prvalue_variable>(
     //   func->get_type().get_return_type().clone(),
      //  Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value, ""));
   // }
    
  //} else {
  //  return nullptr;
  //}
}

std::unique_ptr<build_variable> build_var_exp(base_build &build,
                                                    var_exp const &ast) {
  if (std::unique_ptr<build_scope> p{ast.get_father_scope().build(build)}; p == nullptr) {
    std::cerr << "error in var scope\n";
    return nullptr;
  } else if (auto it = p->find_var(ast.get_name()); it == nullptr) {
    return nullptr;
  } else {
      return it->clone();
  }
}

std::unique_ptr<build_variable>
build_binary_mul_exp(base_build &build, binary_mul_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator*"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value;
std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);
    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator*"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateFMul(l, r));
    } else if (id->is_integral()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateMul(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_binary_div_exp(base_build &build, binary_div_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator/"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator/"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateFDiv(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateSDiv(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateUDiv(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_binary_mod_exp(base_build &build, binary_mod_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator%"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator%"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateFRem(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateSRem(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateURem(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_binary_plus_exp(base_build &build, binary_plus_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator+"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator+"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateFAdd(l, r));
    } else if (id->is_integral()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateAdd(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_binary_minus_exp(base_build &build, binary_minus_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator-"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator-"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateFSub(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateSub(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_three_way_exp(base_build &build, three_way_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator<=>"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator<=>"));
  } else {
    return nullptr;
  }
}

std::unique_ptr<build_variable> build_less_exp(base_build &build,
                                                     less_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator<"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator<"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpOLT(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpSLT(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpULT(l, r));
    }        
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_less_eq_exp(base_build &build, less_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator<="s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator<="));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpOLE(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpSLE(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpULE(l, r));
    }        
    
  } else {
  return nullptr;
  }
}
std::unique_ptr<build_variable>
build_greater_exp(base_build &build, greater_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator>"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator>"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpOGT(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpSGT(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpUGT(l, r));
    }        
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_greater_eq_exp(base_build &build, greater_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator>="s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator>="));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpOGE(l, r));
    } else if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpSGE(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpUGE(l, r));
    }        
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable> build_eq_exp(base_build &build,
                                                   eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator=="s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator=="));
  } else if (arges[0]->get_type().is_bool() && arges[1]->get_type().is_bool()) {
    return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpEQ(arges[0]->get_value(), arges[1]->get_value()));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpOEQ(l, r));
    } else if (id->is_integral()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpEQ(l, r));
    }       
    
  } else {
  return nullptr;
  }
}
std::unique_ptr<build_variable> build_not_eq_exp(base_build &build,
                                                       not_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator!="s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator!="));
  } else if (arges[0]->get_type().is_bool() && arges[1]->get_type().is_bool()) {
    return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpNE(arges[0]->get_value(), arges[1]->get_value()));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_floating_point()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateFCmpONE(l, r));
    } else if (id->is_integral()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateICmpNE(l, r));
    }       
    
  } else {
  return nullptr;
  }
}
std::unique_ptr<build_variable>
build_bitwise_and_exp(base_build &build, bitwise_and_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator&"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator&"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateAnd(l, r));
    }         
    
  } else {
    return nullptr;
  }
}
std::unique_ptr<build_variable>
build_bitwise_xor_exp(base_build &build, bitwise_xor_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator^"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator^"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateXor(l, r));
    }         
    
  } else {
    return nullptr;
  }
}
std::unique_ptr<build_variable>
build_bitwise_or_exp(base_build &build, bitwise_or_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator|"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator|"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateOr(l, r));
    }         
    
  } else {
    return nullptr;
  }
}
std::unique_ptr<build_variable>
build_logical_and_exp(base_build &build, logical_and_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator&&"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator&&"));
  } else if (arges[0]->get_type().is_bool() && arges[1]->get_type().is_bool()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateLogicalAnd(arges[0]->get_value(), arges[1]->get_value()));
  } else {
    return nullptr;
  }
}
std::unique_ptr<build_variable>
build_logical_or_exp(base_build &build, logical_or_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator||"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value{
        arges[0]->get_value(),
        arges[1]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator||"));
  } else if (arges[0]->get_type().is_bool() && arges[1]->get_type().is_bool()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateLogicalOr(arges[0]->get_value(), arges[1]->get_value()));
  } else {
    return nullptr;
  }
}

std::unique_ptr<build_variable>
build_logical_not_exp(base_build &build, logical_not_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 1> arges{build_expression(build, ast.get_arg())};
  
  reference_ptr<function_build> func =
      function_lookup(build, "operator!"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value{
        arges[0]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator!"));
  } else if (arges[0]->get_type().is_bool()) {
      return std::make_unique<build_prvalue_variable>(std::make_unique<bool_type_t>(), 
      Builder->CreateNot(arges[0]->get_value()));
  } else {
    return nullptr;
  }
}


std::unique_ptr<build_variable>
build_bitwise_not_exp(base_build &build, bitwise_not_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 1> arges{build_expression(build, ast.get_arg())};
  
  reference_ptr<function_build> func =
      function_lookup(build, "operator~"s, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value{
        arges[0]->get_value(),
    };

    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator~"));
  } else if (arges[0]->get_type().is_unsigned()) {
    return std::make_unique<build_prvalue_variable>(arges[0]->get_type().clone(), 
          Builder->CreateNot(arges[0]->get_value()));
  } else {
    return nullptr;
  }
}


std::unique_ptr<build_variable>
build_subscript_exp(base_build &build, subscript_exp const &ast) {
  std::unique_ptr<build_variable> exp{ast.get_arr().build(build)};
  
  if (!exp->get_type().is_array()) {
    // TODO logerror
    goto error;
  }

  {

    arr_type const &exp_type{static_cast<arr_type const&>((exp->get_type()))};

    if (ast.get_args().size() != exp_type.m_size.size()) {
      // TODO logerror
      goto error;
    }  

    std::vector<std::unique_ptr<build_variable>> args;
    for (auto const &arg : ast.get_args()) {
        if (!args.emplace_back(arg->build(build))->get_type().is_integral()) {
          // TODO log error
          goto error;
        }
    }
    std::vector<std::size_t> factors;
    std::size_t a{1};
    for (auto it =exp_type.m_size.rbegin(); it !=exp_type.m_size.rend(); ++it) {
      factors.emplace_back(a);
      a *= *it;
    }

    llvm::Value* ini = llvm::ConstantInt::get(llvm::Type::getInt64Ty(*TheContext), 0);
    auto it{factors.rbegin()};
    for (auto const & arg : args) {
      llvm::Value*a = llvm::ConstantInt::get(llvm::Type::getInt64Ty(*TheContext), *it);
      llvm::Value * mul = Builder->CreateMul(a, arg->get_value(), "");
      ini = Builder->CreateAdd(ini, mul, "");
      ++it;
    }  

    return std::make_unique<build_prvalue_variable>(
      exp_type.get_element_type().clone(),
      Builder->CreateGEP(exp_type.get_element_type().get_llvm_type(), exp->get_value(),
                        {ini, llvm::ConstantInt::get(llvm::Type::getInt64Ty(*TheContext), 0)})
    );
   
  }
error:
  return nullptr;

}

std::unique_ptr<build_variable>
build_dot_exp(base_build &build, dot_exp const &ast) {
  std::unique_ptr<build_variable> exp{ast.get_exp().build(build)};
  if (exp == nullptr) {
    goto error;
  }

  if (!exp->get_type().is_struct()) {
    // TODO log error 
    goto error;
  }

  {
    class_type const&exp_type{static_cast<class_type const&>(exp->get_type())};
    
    auto var = exp_type.get_member_var(exp->get_value(), ast.get_id());
    if (var == nullptr) {
      // TODO log error
      goto error;
    }

   /* if (var->get_type().is_reference()) {
      return std::make_unique<build_local_reference>(
        var->get_type().clone(),
        IR::Builder->CreateStructGEP(
          exp_type.get_llvm_type(), exp->get_value(),
          ast.get_id()
          )
        );
    }*/

    // exp is lvalue


    // exp is rvalue

  }


error:
  return nullptr;
}

//template <class Iterator>
//reference_ptr<callable>
//ADL(const base_build &build, std::string_view function_name, Iterator begin, Iterator end){
 // if (auto p{build.find_var(function_name)}; p != nullptr) {
  //  return p;
 // } else {

 // }
  
//}
std::unique_ptr<build_constant_variable> build_literal(base_build &build,
                                                       exp_ast const &ast) {

  auto var = build_expression(build, ast);
  if (llvm::Constant *val = llvm::dyn_cast<llvm::Constant>(var->get_value()); val != nullptr) {
    return std::make_unique<build_constant_variable>(var->get_type().clone(), val);
  } else {
    return nullptr;
  }
  // TODO
}
std::unique_ptr<build_variable> build_arrow_exp(base_build &build,
                                                arrow_exp const &ast) {

  std::unique_ptr<build_variable> exp{ast.get_exp().build(build)};

  if (!exp->get_type().is_pointer()) {
    // TODO logerror
    return nullptr;
  }
  pointer_type const &ptype{static_cast<pointer_type const &>(exp->get_type())};
  if (!ptype.get_element_type().is_struct()) {
    // TODO log error
    return nullptr;
  }
  class_type const &exp_type{
      static_cast<class_type const &>(ptype.get_element_type())};
  llvm::Value *obj =
      Builder->CreateLoad(exp_type.get_llvm_type(), exp->get_value());

  auto var = exp_type.get_member_var(obj, ast.get_id());
  if (var == nullptr) {
    // TODO log error
    return nullptr;
  }

}

std::unique_ptr<build_variable>
build_address_of_exp(base_build &build, address_of_exp const &ast) {
  std::unique_ptr<build_variable> exp{ast.get_arg().build(build)};

  if (!exp->is_lvalue()) {
    // TODO log error
    return nullptr;
  }

  return std::make_unique<build_prvalue_variable>(exp->get_type().clone(), exp->address_of());
}


// std::unique_ptr<build_variable>
// build_allocation_exp(base_build &build, allocation_exp const &ast) {

// }

// 虽然难以解释为什么llvm要求位运算类型相同，这与常见硬件行为、其他语言并不一致，而且额外的高位对于右操作数没有意义，但是目前仍然按照llvm要求实现
std::unique_ptr<build_variable>
build_left_shift_exp(base_build &build, left_shift_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator<<"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value;
std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);
    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator<<"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_integral()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateShl(l, r));
    }         
    
  } else {
  return nullptr;
  }
}

std::unique_ptr<build_variable>
build_right_shift_exp(base_build &build, right_shift_exp const &ast) {
  std::array<std::unique_ptr<build_variable>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator>>"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value;
std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);
    return std::make_unique<build_prvalue_variable>(
        func->get_type().get_return_type().clone(),
        Builder->CreateCall(func->get_type().get_llvm_type(), func->get_value(), args_Value,
                            "call_operator>>"));
  } else if (auto [l,r, id] = usual_arithmetic_conversions(*arges[0], *arges[1]); 
             l != nullptr){
    if (id->is_signed()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateAShr(l, r));
    } else if (id->is_unsigned()) {
      return std::make_unique<build_prvalue_variable>(std::move(id), 
      Builder->CreateLShr(l, r));
    } else {
    return nullptr;
    }      
    
  } else {
  return nullptr;
  }
}
} // namespace pache