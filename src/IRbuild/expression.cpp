#include "expression.h"
#include "IRbuild/variable.h"
#include "build.h"
#include "class_type.h"
#include "function.h"
#include "type.h"
#include "variable.h"
#include "llvm/IR/Value.h"
#include <algorithm>
#include <cstddef>
#include <functional>
#include <iterator>
#include <memory>
#include <vector>

namespace pache {
///std::unique_ptr<build_variable> build_exp(base_build &build,
///                                                exp_ast const &ast) {
//  return ast.build(build);
//}

std::unique_ptr<build_variable> build_expression(base_build &build,
                                                       exp_ast const &ast) {
  return ast.build(build);
}

std::unique_ptr<build_variable> build_unary_plus(base_build &build,
                                                       unary_plus const &ast) {
  std::array<std::unique_ptr<build_variable>, 1> arges{
      build_expression(build, ast.get_arg())};

  if (any_of(arges.begin(), arges.end(),
             [](std::unique_ptr<build_variable> &ptr) -> bool {
               return ptr == nullptr;
             })) {
    // args have problem the error has logged

    return std::unique_ptr<build_variable>(nullptr);
  }

  reference_ptr<function_build> func =
      function_lookup(build, "operator+"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    return std::make_unique<build_prvalue_variable>(
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator+"));
  } else {
    // TODO log error

    return std::unique_ptr<build_variable>(nullptr);
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

    return std::unique_ptr<build_variable>(nullptr);
  }

  reference_ptr<function_build> func =
      function_lookup(build, "operator-"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    return std::make_unique<build_prvalue_variable>(
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator-"));
  } else {
    // TODO log error

    return std::unique_ptr<build_variable>(nullptr);
  }
}

std::unique_ptr<build_variable>
build_func_call_exp(base_build &build, func_call_exp const &ast) {
  

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

   // if (func->get_function_type().get_return_type()->is_reference()) {
    //  return std::make_unique<build_local_reference>(
     //   func->get_function_type().get_return_type()->clone(),
      //  Builder->CreateCall(func->get_llvm_function(), args_Value, ""));
   // } else {
    //  return std::make_unique<build_prvalue_variable>(
     //   func->get_function_type().get_return_type()->clone(),
      //  Builder->CreateCall(func->get_llvm_function(), args_Value, ""));
   // }
    
  //} else {
  //  return std::unique_ptr<build_variable>(nullptr);
  //}
}

std::unique_ptr<build_variable> build_var_exp(base_build &build,
                                                    var_exp const &ast) {
  if (build.find_var(ast.get_name()) != nullptr) {
    return build.find_var(ast.get_name());
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator*"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator/"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator%"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator+"));
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator-"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<=>"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<="));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator>"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator>="));
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator=="));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator!="));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator&"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator^"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator|"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator&&"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
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
        func->get_function_type().get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator||"));
  } else {
    return std::unique_ptr<build_variable>(nullptr);
  }
}

std::unique_ptr<build_variable>
build_subscript_exp(base_build &build, subscript_exp const &ast) {
  std::unique_ptr<build_variable> exp{ast.get_arr().build(build)};
  
  if (!exp->get_type()->is_array()) {
    // TODO logerror
    goto error;
  }

  {

    arr_type const &exp_type{static_cast<arr_type const&>(*(exp->get_type()))};

    if (ast.get_args().size() != exp_type.m_size.size()) {
      // TODO logerror
      goto error;
    }  

    std::vector<std::unique_ptr<build_variable>> args;
    for (auto const &arg : ast.get_args()) {
        if (!args.emplace_back(arg->build(build))->get_type()->is_integral()) {
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

  if (!exp->get_type()->is_struct()) {
    // TODO log error 
    goto error;
  }

  {
    class_type const&exp_type{static_cast<class_type const&>(*exp->get_type())};
    
    auto var = exp_type.get_member_var(ast.get_id());
    if (var == nullptr) {
      // TODO log error
      goto error;
    }

    if (var->get_type().is_reference()) {
      return std::make_unique<build_local_reference>(
        var->get_type().clone(),
        IR::Builder->CreateGEP(
          exp_type.get_llvm_type(), exp->get_value(),
          {IR::Builder->getInt32(0), IR::Builder->getInt32(var->get_num())},
          ast.get_id()
          )
        );
    }

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
} // namespace pache