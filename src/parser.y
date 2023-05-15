%require "3.2"
%language "c++"

%define api.token.raw

%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <memory>
  #include <string>
  #include "../ast/ast.h"
  #include "../ast/expression.h"
  #include "../ast/statement.h"
  #include "../ast/compunit.h"
}

%parse-param { std::unique_ptr<pache::base_ast> &ast }

%locations

%define api.token.prefix {TOK_}
// Generate the parser as `::pache::parser`.
%define api.namespace { pache }
%define api.parser.class { parser }

// Make parse error messages more detailed
//%define parse.error verbose

// Enable support for parser debugging
%define parse.trace
%define parse.error detailed
%define parse.lac full



%code {
#include <iostream>
#include <memory>
#include <string>
#include "../ast/ast.h"
#include "../ast/expression.h"
#include "../ast/statement.h"
#include "../ast/type.h"
#include "../src/driver.h"
#include "../ast/literal.h"
#include "../ast/compunit.h"
// 声明 lexer 函数和错误处理函数

using namespace std;

}

%token
  VOID                 "void"

  BOOL                 "bool"

  SIZE                 "size"

  I8                   "i8"
  I16                  "i16"
  I32                  "i32"
  I64                  "i64"
  I128                 "i128"

  U8                   "u8"
  U16                  "u16"
  U32                  "u32"
  U64                  "u64"
  U128                 "u128"

  F16                  "f16"
  F32                  "f32"
  F64                  "f64"
  F128                 "f128"

  D32                  "d32"
  D64                  "d64"

  C8                   "c8"
  C16                  "c16"
  C32                  "c32"
%token
  B_AND                "&"
  B_XOR                "^"
  B_OR                 "|"

  ASSIGN                // :=
  LEFT_CURLY_BRACE      // {
  LEFT_PARENTHESIS      // (
  LEFT_SQUARE_BRACKET   // [
  RIGHT_PARENTHESIS     // )
  RIGHT_CURLY_BRACE     // }
  RIGHT_SQUARE_BRACKET  // ]
  PLUS                  // +
  MINUS                 // -
  PERIOD                // .
  SLASH                 // /
  PERCENT               // %
  THREE_WAY_COMPARISON  // <=>
  LESS                  // <
  LESS_EQUAL            // <=
  GREATER               // >
  GREATER_EQUAL         // >=
  EQUAL                 // ==
  NOT_EQUAL             // !=
  AND                   // &&
  OR                    // ||
  MAIN                  // main
  LET                   // let
  IF                    // if
  ELSE                  // else
  LOOP                  // loop
  BREAK                 // break
  CONTINUE              // continue
  COMMA                 // ,
  SEMICOLON             // ;
  NEW_LINE              // \n
  UNARY_STAR "unary *"
  PREFIX_STAR "prefix *"
  POSTFIX_STAR "postfix *"
  BINARY_STAR "binary *"
%token RETURN FUNC CLASS
%token <std::string> IDENTIFIER
%token <int> INTEGER
%token EOF 0;
// 非终结符的类型定义
%type <pache::compunit_ast*>   CompUnit
%type <pache::variable_ast*> FuncDef main_func
%type <pache::type_ast const*> type arr_type
%type <pache::exp_ast*> Number   primary_expression unary_expression call_expression
%type <pache::block_ast*> block optional_else if_stmt loop_stmt
%type <pache::stmt_ast*> stmt return_stmt let_stmt assign_stmt break_stmt continue_stmt
%type <pache::exp_ast*>  expression  add_exp
%type <std::vector<pache::stmt_ast*>> statement_list
%type <pache::exp_ast*>  mul_expressions
%type <pache::exp_ast*> three_way_expression rel_expression eq_expression logical_and_expression logical_or_expression
%type <pache::func_arg*> FuncFParam
%type <std::vector<pache::func_arg*>> FuncFParams
%type <std::vector<pache::exp_ast*>> FuncRParams
%type <pache::class_ast*> class_def
%type <std::vector<pache::base_ast*>> class_body
%type <pache::exp_ast*> bitwise_and_expression
%type <pache::exp_ast*> bitwise_xor_expression
%type <pache::exp_ast*> bitwise_or_expression


%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : FuncDef {
    ast = make_unique<pache::compunit_ast>();
    ast->insert_dec($1);
  }
| let_stmt {
    ast = make_unique<pache::compunit_ast>();
    ast->insert_dec($1);
}
| class_def {
    ast = make_unique<pache::compunit_ast>();
    ast->insert_class_def($1);
}
| CompUnit FuncDef {
      ast->insert_dec($2);
}
| CompUnit let_stmt {
    ast->insert_dec($2);
}
| CompUnit class_def {
  ast->insert_class_def($2);
};

FuncFParam:
  type IDENTIFIER {
    $$ = new pache::func_arg($1, std::move($2));
  };

FuncFParams:
  // empty

  { $$ = std::vector<pache::func_arg*>();
  }
  | FuncFParam {
    $$ = std::vector<pache::func_arg*>();
    $$.push_back($1);
  }
| FuncFParams COMMA FuncFParam {
      $$ = $1;
      $$.push_back($3);

};
main_func:
  FUNC MAIN LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS I32 block {
    auto ast = new pache::main_func_ast("main"s, $4, pache::i32_type_t::get(), $7);

     // for (auto arg : *$4) {
     //   arg->set_father($$);
     // }
    $$ = ast;
  };

FuncDef
  : FUNC IDENTIFIER LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS type block {
    auto ast = new pache::func_ast(std::move($2), $4, $6, $7);
    //      for (auto arg : *$4) {
     //   arg->set_father($$);
     // }
    $$ = ast;
  }
| main_func {
  $$ = $1;
};

class_body:
  // Empty
  { $$ = {}; }
| class_body let_stmt {
  $$ = $1;
  $$.push_back($2);
}
| class_body FuncDef {
  $$ = $1;
  $$.push_back($2);
};

class_def:
  CLASS IDENTIFIER LEFT_CURLY_BRACE class_body RIGHT_CURLY_BRACE
  {
    $$ = new class_ast(std::move($2), std::move($4));
  };
// 同上, 不再解释
type:
  VOID
  { $$ = pache::void_type_t::get(); }
| BOOL {
    $$ = pache::bool_type_t::get();
  }
| SIZE {
  $$ = pache::size_type_t::get();
}
| I8
{ $$ = pache::i8_type_t::get();}
| I16
{ $$ = pache::i16_type_t::get(); }
| I32
{ $$ = pache::i32_type_t::get(); }
| I64
{ $$ = pache::i64_type_t::get();}
| I128
{ $$ = pache::i128_type_t::get(); }
| U8
{ $$ = pache::u8_type_t::get();}
| U16
{ $$ = pache::u16_type_t::get(); }
| U32
{ $$ = pache::u32_type_t::get(); }
| U64
{ $$ = pache::u64_type_t::get();}
| U128
{ $$ = pache::u128_type_t::get(); }
| F16
{ $$ = pache::f16_type_t::get(); }
| F32
{ $$ = pache::f32_type_t::get(); }
| F64
{ $$ = pache::f64_type_t::get();}
| F128
{ $$ = pache::f128_type_t::get(); }
| D32
{ $$ = pache::d32_type_t::get(); }
| D64
{ $$ = pache::d64_type_t::get();}
| C8
{ $$ = pache::c8_type_t::get();}
| C16
{ $$ = pache::c16_type_t::get(); }
| C32
{ $$ = pache::c32_type_t::get(); }
| arr_type {
  $$ = $1;
}
| IDENTIFIER {
  $$ = new pache::user_def_type(std::move($1));
}
  ;

arr_type:
  type LEFT_SQUARE_BRACKET INTEGER RIGHT_SQUARE_BRACKET {
    $$ = new pache::arr_type_t($1, $3);
  }

statement_list:
  // Empty
    { $$ = std::vector<pache::stmt_ast*>{}; }
| statement_list stmt
    {
      $$ = $1;;
      $$.push_back($2);
    }
;
block
  : LEFT_CURLY_BRACE statement_list RIGHT_CURLY_BRACE {
    $$ = new pache::block_ast(std::move($2));
    for (auto & ast : $2) {
      ast->set_father($$);
    }
  }
  ;

stmt:
  assign_stmt {
  $$ = $1;
}
| expression SEMICOLON {
  $$ = new pache::exp_stmt($1);
}
| block
    { $$ = $1; }
| let_stmt {
    $$ = $1;
}
| return_stmt
    { $$ = $1; }
| if_stmt {
  $$ = $1;
}
| loop_stmt {
  $$ = $1;
}
| break_stmt {
  $$ = $1;
}
| continue_stmt {
  $$ = $1;
}
;



return_stmt:
  RETURN SEMICOLON {
    $$ = new pache::return_ast(&pache::void_literal);
  }
| RETURN expression SEMICOLON {
    $$ = new pache::return_ast($2);
  }
  ;

let_stmt:
  LET type IDENTIFIER ASSIGN expression SEMICOLON {
    $$ = new pache::let_stmt(new pache::variable_ast($2, $3), $5);
  }
| LET type IDENTIFIER LEFT_CURLY_BRACE expression RIGHT_CURLY_BRACE SEMICOLON {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var, $5);
}
| LET type IDENTIFIER SEMICOLON {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var, nullptr);
  }
;

assign_stmt:
  IDENTIFIER ASSIGN expression SEMICOLON {
    auto exp = new pache::var_exp(std::move($1));
    $$ = new pache::assign_stmt(exp, $3);
  }
;

break_stmt:
  BREAK SEMICOLON {
    $$ = new pache::break_stmt();
  };

continue_stmt:
  CONTINUE SEMICOLON {
    $$ = new pache::continue_stmt();
  };
optional_else:
  // Empty
    { $$ = nullptr; }
| ELSE if_stmt
    {
      $$ = $2;
    }
| ELSE block
    { $$ = $2; }
;

if_stmt:
  IF expression block optional_else {
    $$ = new pache::if_stmt($2, $3, $4);
  }
Number
  : INTEGER {
    $$ = new pache::i32_literal($1);
  }
  ;

loop_stmt:
  LOOP block {
    $$ = new pache::loop_stmt($2);

  }
;
expression:
  logical_or_expression {
    $$ = $1;
  }
;

FuncRParams:
  // empty
  {
    $$ = std::vector<pache::exp_ast*>();
  }|
  expression {
    $$ = std::vector<pache::exp_ast*>{};
    $$.push_back($1);
  }
| FuncRParams COMMA expression {
  $$ = $1;
  $$.push_back($3);
};

primary_expression:
  LEFT_PARENTHESIS expression RIGHT_PARENTHESIS {
    $$ = $2;
  }
| Number {
    $$ = $1;
  }
| IDENTIFIER {
    $$ = new pache::var_exp(std::move($1));
}
;

call_expression:
  primary_expression {
    $$ = $1;
  }
| IDENTIFIER LEFT_PARENTHESIS FuncRParams RIGHT_PARENTHESIS {
  $$ = new pache::func_call_exp(std::move($1), std::move($3));
}

unary_expression:
  call_expression {
    $$ = $1;
  }
| PLUS unary_expression {
    $$ = new pache::unary_plus($2);
  }
| MINUS unary_expression {
    $$ = new pache::unary_minus($2);
  }
;


mul_expressions:
  unary_expression {
    $$ = $1;
  }
| mul_expressions BINARY_STAR unary_expression {
    $$ = new pache::binary_mul_exp($1, $3);
  }
| mul_expressions SLASH unary_expression {
    $$ = new pache::binary_div_exp($1, $3);
  }
| mul_expressions PERCENT unary_expression {
    $$ = new pache::binary_mod_exp($1, $3);
  }
;

add_exp:
  mul_expressions {
    $$ = $1;
  }
| add_exp PLUS mul_expressions {
    $$ = new pache::binary_plus_exp($1, $3);
  }
| add_exp MINUS mul_expressions {
    $$ = new pache::binary_minus_exp($1, $3);
  }
;

three_way_expression:
  add_exp {
    $$ = $1;
  }
| three_way_expression THREE_WAY_COMPARISON add_exp {
    $$ = new pache::three_way_exp($1, $3);
}
;

rel_expression:
  three_way_expression {
    $$ = $1;
  }
| rel_expression LESS three_way_expression {
    $$ = new pache::less_exp($1, $3);
}
| rel_expression LESS_EQUAL three_way_expression {
  $$ = new pache::less_eq_exp($1, $3);
}
| rel_expression GREATER three_way_expression {
  $$ = new pache::greater_exp($1, $3);
}
| rel_expression GREATER_EQUAL three_way_expression {
  $$ = new pache::greater_eq_exp($1, $3);
}
;

eq_expression:
  rel_expression {
    $$ = $1;
  }
| eq_expression EQUAL rel_expression {
  $$ = new pache::eq_exp($1, $3);
}
| eq_expression NOT_EQUAL rel_expression {
  $$ = new pache::not_eq_exp($1, $3);
}
;

bitwise_and_expression:
  eq_expression {
    $$ = $1;
  }
| bitwise_and_expression B_AND eq_expression {
  $$ = new pache::bitwise_and_exp($1, $3);
};

bitwise_xor_expression:
  bitwise_and_expression {
    $$ = $1;
  }
| bitwise_xor_expression B_XOR bitwise_and_expression {
  $$ = new pache::bitwise_xor_exp($1, $3);
}
;
bitwise_or_expression:
  bitwise_xor_expression {
    $$ = $1;
  }
| bitwise_or_expression B_OR bitwise_xor_expression {
  $$ = new pache::bitwise_or_exp($1, $3);
}
;
logical_and_expression:
  bitwise_or_expression {
    $$ = $1;
  }
| logical_and_expression AND bitwise_or_expression {
  $$ = new pache::logical_and_exp($1, $3);
}
;

logical_or_expression:
  logical_and_expression {
    $$ = $1;
  }
| logical_or_expression OR logical_and_expression {
  $$ = new pache::logical_or_exp($1, $3);
}
;






%%

// 定义错误处理函数, 其中第二个参数是错误信息
// parser 如果发生错误 (例如输入的程序出现了语法错误), 就会调用这个函数
namespace pache
{
  // Report an error to the user.
  auto parser::error (const location& l, const std::string& msg) -> void
  {
    std::cerr << msg << '\n';
  }
}
