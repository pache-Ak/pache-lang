#include "expression.h"
#include "IRbuild/variable.h"
#include "function.h"
#include "variable.h"
#include <algorithm>
#include <functional>
#include <iterator>
#include <memory>

namespace pache {
std::unique_ptr<build_variable const> build_exp(base_build &build,
                                                exp_ast const &ast) {
  return ast.build(build);
}
std::unique_ptr<build_variable const> build_expression(base_build &build,
                                                       exp_ast const &ast) {
return ast.build(build);
                                                       }
std::unique_ptr<build_variable const> build_unary_plus(base_build &build,
                                                       unary_plus const &ast) {
  std::array<std::unique_ptr<build_variable const>, 1> arges{
      build_expression(build, ast.get_arg())};

  if (any_of(arges.begin(), arges.end(),
             [](std::unique_ptr<build_variable const> &ptr) -> bool {
               return ptr == nullptr;
             })) {
    // args have problem the error has logged

    return std::unique_ptr<build_variable const>(nullptr);
  }

  reference_ptr<function_build> func =
      function_lookup(build, "operator+"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    return std::make_unique<build_prvalue_variable const>(
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator+"));
  } else {
    // TODO log error

    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_unary_minus(base_build &build, unary_minus const &ast) {
  std::array<std::unique_ptr<build_variable const>, 1> arges{
      build_expression(build, ast.get_arg())};

  if (any_of(arges.begin(), arges.end(),
             [](std::unique_ptr<build_variable const> &ptr) -> bool {
               return ptr == nullptr;
             })) {
    // args have problem the error has logged

    return std::unique_ptr<build_variable const>(nullptr);
  }

  reference_ptr<function_build> func =
      function_lookup(build, "operator-"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 1> args_Value;
    std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);

    return std::make_unique<build_prvalue_variable const>(
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator-"));
  } else {
    // TODO log error

    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_func_call_exp(base_build &build, func_call_exp const &ast) {
  using namespace std::placeholders;
  std::vector<std::unique_ptr<build_variable const>> args;
  std::transform(ast.get_args().begin(), ast.get_args().end(), std::back_inserter(args), std::bind(build_expression, std::ref(build), 
  std::bind([](std::unique_ptr<exp_ast> const &ptr)->exp_ast const &{return *ptr;}, _1)));

  reference_ptr<function_build> func =
      function_lookup(build, "func_call"sv, args.begin(), args.end());

  if (func != nullptr) {
    std::vector<llvm::Value *> args_Value;
    std::transform(args.begin(), args.end(), std::back_inserter(args_Value), get_value);

    if (func->get_function_type()->get_return_type()->is_reference()) {
      return std::make_unique<build_local_reference>(
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value, ""));
    } else {
      return std::make_unique<build_prvalue_variable>(
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value, ""));
    }
    
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const> build_var_exp(base_build &build,
                                                    var_exp const &ast) {
  if (build.find_var(ast.get_name()) != nullptr) {
    return build.find_var(ast.get_name());
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_binary_mul_exp(base_build &build, binary_mul_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
      build_expression(build, ast.get_lhs()),
      build_expression(build, ast.get_rhs()),
  };

  reference_ptr<function_build> func =
      function_lookup(build, "operator*"sv, arges.begin(), arges.end());

  if (func != nullptr) {
    std::array<llvm::Value *, 2> args_Value;
std::transform(arges.begin(), arges.end(), args_Value.begin(), get_value);
    return std::make_unique<build_prvalue_variable>(
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator*"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_binary_div_exp(base_build &build, binary_div_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator/"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_binary_mod_exp(base_build &build, binary_mod_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator%"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_binary_plus_exp(base_build &build, binary_mod_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator+"));
  } else {
    return nullptr;
  }
}

std::unique_ptr<build_variable const>
build_binary_minus_exp(base_build &build, binary_minus_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator-"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_three_way_exp(base_build &build, three_way_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<=>"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const> build_less_exp(base_build &build,
                                                     less_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_less_eq_exp(base_build &build, less_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator<="));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_greater_exp(base_build &build, greater_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator>"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_greater_eq_exp(base_build &build, greater_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator>="));
  } else {
    return nullptr;
  }
}

std::unique_ptr<build_variable const> build_eq_exp(base_build &build,
                                                   eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator=="));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const> build_not_eq_exp(base_build &build,
                                                       not_eq_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator!="));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_bitwise_and_exp(base_build &build, bitwise_and_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator&"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_bitwise_xor_exp(base_build &build, bitwise_xor_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator^"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_bitwise_or_exp(base_build &build, bitwise_or_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator|"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_logical_and_exp(base_build &build, logical_and_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator&&"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}
std::unique_ptr<build_variable const>
build_logical_or_exp(base_build &build, logical_or_exp const &ast) {
  std::array<std::unique_ptr<build_variable const>, 2> arges{
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
        func->get_function_type()->get_return_type()->clone(),
        Builder->CreateCall(func->get_llvm_function(), args_Value,
                            "call_operator||"));
  } else {
    return std::unique_ptr<build_variable const>(nullptr);
  }
}

std::unique_ptr<build_variable const>
build_subscript_exp(base_build &build, subscript_exp const &ast) {
 
}
} // namespace pache