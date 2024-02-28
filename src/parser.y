%require "3.2"
%language "c++"

%define api.token.raw
%define api.token.constructor
%define api.value.type variant
%define parse.assert

%code requires {
  #include <memory>
  #include <string>
  namespace pache {
    class driver;
  }
  #include "ast/ast.h"
  #include "ast/expression.h"
  #include "ast/statement.h"
  #include "ast/compunit.h"
  #include "ast/scope.h"
}

%parse-param { pache::driver &drv }
%lex-param {pache::driver &drv}
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
  #include "ast/ast.h"
  #include "ast/expression.h"
  #include "ast/statement.h"
  #include "ast/compunit.h"

#include "ast/type.h"
#include "ast/multi_array_type.h"
#include "driver.h"
#include "ast/literal.h"
}

%token
  HORIZONTAL_WHITESPACE
  WHITESPACE 

  SCOPE                 // ::
  BITWISE_NOT          "~"
  B_AND                "&"
  B_XOR                "^"
  B_OR                 "|"
  LEFT_SHITF            // <<
  RIGHT_SHIFT           // >>
  ASSIGN                // :=
  LEFT_CURLY_BRACE      // {
  LEFT_PARENTHESIS      // (
  LEFT_SQUARE_BRACKET   // [
  RIGHT_PARENTHESIS     // )
  RIGHT_CURLY_BRACE     // }
  RIGHT_SQUARE_BRACKET  // ]
  PLUS                  // +
  BINARY_PLUS
  LOGICAL_NOT           // !
  MINUS                 // -
  BINARY_MINUS
  DOT                   // .
  ARROW                 // ->
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
  MUTABLE                 // mut
  VOLATILE              // volatile
  SINGLE_QUOTE          // '
  ALLOCATION            // new
  DEALLOCATION          // delete
  UNARY_STAR            // *
  BINARY_STAR           //  *
  BINARY_PREFIX         // 0b 0B
  HEXADECIMAL_PREFIX    // 0x 0X
  ZERO                          // 0
%token RETURN FUNC CLASS
%token <std::string> IDENTIFIER 
%token <int> INTEGER
%token <char const *> BINARY_DIGIT OCTAL_DIGIT NONZERO_DIGIT DECIMAL_DIGIT HEXADECIMAL_DIGIT DIGIT
%token EOF 0;
// 非终结符的类型定义
%type <pache::named_ast>namespace_name
%type <std::unique_ptr<pache::compunit_ast>>   CompUnit
%type <std::unique_ptr<pache::type_ast>> type mut_ast volatile_ast multi_array_ast primary_type
%type <std::unique_ptr<pache::exp_ast>>    primary_expression unary_expression call_expression
%type <std::unique_ptr<pache::block_ast>> block /*optional_else*/  loop_stmt
%type <std::unique_ptr<pache::let_stmt>> let_stmt
%type <std::unique_ptr<pache::stmt_ast>> stmt return_void_stmt return_stmt  if_stmt if_else_stmt assign_stmt break_stmt continue_stmt else_stmt
%type <std::unique_ptr<pache::exp_ast>>  expression  add_exp identification_expression
%type <std::vector<std::unique_ptr<pache::stmt_ast>>> statement_list
%type <std::vector<std::unique_ptr<pache::exp_ast>>> FuncRParams
%type <std::unique_ptr<pache::exp_ast>>  mul_expressions shift_exp
%type <std::unique_ptr<pache::exp_ast>> three_way_expression rel_expression eq_expression logical_and_expression logical_or_expression
%type <std::pair<std::unique_ptr<pache::type_ast>, std::string>> FuncFParam
%type <std::pair<std::vector<std::unique_ptr<pache::type_ast>>, std::vector<std::string>>> FuncFParams
%type <std::unique_ptr<pache::class_ast>> class_def
%type <pache::class_ast::class_body> class_body
%type <std::unique_ptr<pache::exp_ast>> bitwise_and_expression
%type <std::unique_ptr<pache::exp_ast>> bitwise_xor_expression
%type <std::unique_ptr<pache::exp_ast>> bitwise_or_expression
%type <std::unique_ptr<pache::func_ast>> FuncDef  // main_func
%type <std::vector<std::unique_ptr<pache::exp_ast>>> size_list
%type <std::unique_ptr<pache::scope_ast>> scope_resolution
%type <std::unique_ptr<pache::exp_ast>> LITERAL INTEGER_LITERAL
%type <std::string> binary_digit_sequence octal_digit_sequence decimal_digit_sequence hexadecimal_digit_sequence
%type <std::string> binary_literal octal_literal decimal_literal hexadecimal_literal
%%
%start unit;
unit : CompUnit {
  drv.root_ast = std::move($1);
}
;

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : FuncDef {
    $$ = std::make_unique<pache::compunit_ast>();
    $$->insert_func_def(std::move($1));
  }
| let_stmt {
    $$ = std::make_unique<pache::compunit_ast>();
    $$->insert_dec(std::move($1));
}
| class_def {
    $$ = std::make_unique<pache::compunit_ast>();
    $$->insert_class_def(std::move($1));
}
| WHITESPACE {
  $$ = std::make_unique<pache::compunit_ast>();
}
| CompUnit WHITESPACE {
  $$ = std::move($1);
}
| CompUnit FuncDef {
  $$ = std::move($1);
      $$->insert_func_def(std::move($2));
}
| CompUnit let_stmt {
  $$ = std::move($1);
    $$->insert_dec(std::move($2));
}
| CompUnit class_def {
  $$ = std::move($1);
  $$->insert_class_def(std::move($2));
};

FuncFParam:
  type WHITESPACE IDENTIFIER {
    $$ = std::make_pair<std::unique_ptr<pache::type_ast>,std::string>(std::move($1), std::move($3));
  };

FuncFParams:
  %empty
  { $$ = std::make_pair<std::vector<std::unique_ptr<pache::type_ast>>, std::vector<std::string>>({}, {});
  }
  | FuncFParam {
    $$ = std::make_pair<std::vector<std::unique_ptr<pache::type_ast>>, std::vector<std::string>>({}, {});
    $$.first.push_back(std::move($1.first));
    $$.second.push_back($1.second);
  }
| FuncFParams COMMA WHITESPACE FuncFParam {
      $$ = std::move($1);
      $$.first.push_back(std::move($4.first));
    $$.second.push_back($4.second);

};
//main_func:
//  FUNC MAIN LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS I32 block {
//    auto ast = std::make_unique<pache::main_func_ast>("main"s, $4, pache::i32_type_t::get(), $7);

     // for (auto arg : *$4) {
     //   arg->set_father($$);
     // }
 //   $$ = ast;
  //};

FuncRParams:
%empty
  {
    $$ = std::vector<std::unique_ptr<pache::exp_ast>>();
  }|
  expression {
    $$ = std::vector<std::unique_ptr<pache::exp_ast>>{};
    $$.emplace_back(std::move($1));
  }
| FuncRParams COMMA expression {
  $$ = std::move($1);
  $$.emplace_back(std::move($3));
};


FuncDef
  : FUNC WHITESPACE IDENTIFIER LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS WHITESPACE type WHITESPACE LEFT_CURLY_BRACE  statement_list  RIGHT_CURLY_BRACE {

    $$ = std::make_unique<func_ast>(std::move($3), std::move($5), std::move($8), std::move($11));
  }
//| main_func {
//  $$ = std::move($1);
//}
;

class_body:
%empty  { $$; }
| class_body let_stmt {
  $$ = std::move($1);
  $$.var_def.emplace_back(std::move($2));
}
| class_body FuncDef {
  $$ = std::move($1);
  $$.func_def.emplace_back(std::move($2));
}
| class_body class_def {
  $$ = std::move($1);
  $$.inner_class_def.emplace_back(std::move($2));

}
| class_body WHITESPACE {
  $$ = std::move($1);
};

class_def:
  CLASS WHITESPACE IDENTIFIER  WHITESPACE LEFT_CURLY_BRACE class_body RIGHT_CURLY_BRACE
  {
    $$ = std::make_unique<pache::class_ast>(std::move($3), std::move($6));
  };
// 同上, 不再解释
mut_ast:
  type WHITESPACE MUTABLE {
    $$ = std::move($1);
  }
;

volatile_ast:
  type WHITESPACE VOLATILE {
    $$ = std::move($1);
  }
;

type :
mut_ast {
  $$ = std::move($1);
} |
volatile_ast {
  $$ = std::move($1);
}|
primary_type {
  $$ = std::move($1);
};

primary_type:
  namespace_name {
    $$ = std::make_unique<pache::named_type_ast>(std::move($1));
  }
 |
multi_array_ast {
$$ = std::move($1);
}
  ;


size_list:
  expression COMMA {
    $$ = std::vector<std::unique_ptr<pache::exp_ast>>{};
    $$.emplace_back(std::move($1));
  } |
  size_list WHITESPACE  expression  COMMA {
    $$ = std::move($1);
    $$.emplace_back(std::move($3));
  }
  ;

  multi_array_ast:
  type LEFT_SQUARE_BRACKET size_list RIGHT_SQUARE_BRACKET {
    $$ = std::make_unique<pache::multi_array_ast>(std::move($1), std::move($3));
  };
statement_list:
  %empty
    { $$ = std::vector<std::unique_ptr<pache::stmt_ast>>{}; }
| statement_list WHITESPACE 
    {
      $$ = std::move($1);
    }
| statement_list WHITESPACE stmt
    {
      $$ = std::move($1);
      $$.emplace_back(std::move($3));
    }
;
block
  : LEFT_CURLY_BRACE WHITESPACE statement_list WHITESPACE RIGHT_CURLY_BRACE {
    $$ = std::make_unique<pache::block_ast>(std::move($3));
  //  for (auto & ast : $2) {
   //   ast->set_father($$->get_father());
   // }
  }

  ;

stmt:
  assign_stmt {
  $$ = std::move($1);
}
| expression SEMICOLON {
  $$ = std::make_unique<pache::exp_stmt>(std::move($1));
}
| block
    { $$ = std::move($1); }
| let_stmt {
    $$ = std::move($1);
}
| return_void_stmt {
  $$ = std::move($1);
}
| return_stmt
    { $$ = std::move($1); }
| loop_stmt {
  $$ = std::move($1);
}
| break_stmt {
  $$ = std::move($1);
}
| continue_stmt {
  $$ = std::move($1);
}  | if_stmt {
    $$ = std::move($1);
  }
;

return_void_stmt:
  RETURN SEMICOLON {
    $$ = std::make_unique<pache::return_void_stmt>();
  }
  ;

return_stmt:
  RETURN WHITESPACE expression SEMICOLON {
    $$ = std::make_unique<pache::return_ast>(std::move($3));
  }
  ;

let_stmt:
LET WHITESPACE type WHITESPACE IDENTIFIER LEFT_CURLY_BRACE expression RIGHT_CURLY_BRACE SEMICOLON  {
    $$ = std::make_unique<pache::let_stmt>(std::move($3), std::move($5), std::move($7));
}
| LET WHITESPACE type WHITESPACE IDENTIFIER SEMICOLON  {
    $$ = std::make_unique<pache::let_stmt>(std::move($3), std::move($5), nullptr);
  }
;

assign_stmt:
  primary_expression WHITESPACE ASSIGN WHITESPACE expression SEMICOLON {
    $$ = std::make_unique<pache::assign_stmt>(std::move($1), std::move($5));
  }
;

break_stmt:
  BREAK SEMICOLON {
    $$ = std::make_unique<pache::break_stmt>();
  };

continue_stmt:
  CONTINUE SEMICOLON {
    $$ = std::make_unique<pache::continue_stmt>();
  };
/* optional_else:
%empty
    { $$ = nullptr; }
//| ELSE if_stmt
  //  {
 //     $$ = $2;
//    }
| ELSE block
    { $$ = $2; }
;
 */
else_stmt:
  block {
    $$ = std::move($1);
  } |
  if_stmt {
    $$ = std::move($1);
  };

if_stmt:
  IF WHITESPACE expression WHITESPACE block  {
    $$ = std::make_unique<pache::if_stmt>(std::move($3), std::move($5));
  }
  | if_else_stmt {
    $$=std::move($1);
  }
  ;
if_else_stmt:
  IF expression block ELSE else_stmt {
    $$ = std::make_unique<pache::if_else_stmt>(std::move($2), std::move($3), std::move($5));
  }



loop_stmt:
  LOOP block {
    $$ = std::make_unique<pache::loop_stmt>(std::move(*($2.release())));

  }
;
expression:
  logical_or_expression {
    $$ = std::move($1);
  }
;

namespace_name: IDENTIFIER {
 $$ =  pache::named_ast(std::make_unique<unqualified_scope_ast>(), std::move($1));
}
| scope_resolution IDENTIFIER {
  $$ = pache::named_ast(std::move($1), std::move($2));
};


scope_resolution
: SCOPE {
  $$ = std::make_unique<root_scope_ast>();
}
| IDENTIFIER SCOPE {
  $$ = std::make_unique<relative_scope_ast>(std::make_unique<unqualified_scope_ast>(), std::move($1));
}
| scope_resolution IDENTIFIER SCOPE {
  $$ = std::make_unique<relative_scope_ast>(std::move($1), std::move($2));
}
;
primary_expression:
  LEFT_PARENTHESIS expression RIGHT_PARENTHESIS {
    $$ = std::move($2);
  }
| LITERAL {
    $$ = std::move($1);
  }
| identification_expression {
  $$ = std::move($1);
}
;

LITERAL
: INTEGER_LITERAL {
  $$ = std::move($1);
}
// | charactr | floating-point | string | boolean | user_defined
;

INTEGER_LITERAL
: binary_literal  {
  $$ = std::make_unique<binary_integer_literal>(std::move($1));
}
| octal_literal  {
  $$ = std::make_unique<octal_integer_literal>(std::move($1));
}
| decimal_literal  {
  $$ = std::make_unique<decimal_integer_literal>(std::move($1));
}
| hexadecimal_literal  {
  $$ = std::make_unique<hexadecimal_integer_literal>(std::move($1));
}
| binary_literal IDENTIFIER {
  $$ = std::make_unique<binary_integer_literal>(std::move($1), std::move($2));
}
| octal_literal IDENTIFIER {
  $$ = std::make_unique<octal_integer_literal>(std::move($1), std::move($2));
}
| decimal_literal IDENTIFIER {
  $$ = std::make_unique<decimal_integer_literal>(std::move($1), std::move($2));
}
| hexadecimal_literal IDENTIFIER {
  $$ = std::make_unique<hexadecimal_integer_literal>(std::move($1), std::move($2));
}

binary_literal
: BINARY_PREFIX binary_digit_sequence {
  $$ = std::move($2);
}

binary_digit_sequence
: BINARY_DIGIT {
  $$ = std::string{};
  $$ += $1;
}
| binary_digit_sequence BINARY_DIGIT {
  $$ = std::move($1);
  $$ += $2;
}
| binary_digit_sequence SINGLE_QUOTE BINARY_DIGIT {
  $$ = std::move($1);
  $$ += $3;
}
;

octal_literal
: octal_digit_sequence
;

octal_digit_sequence
: ZERO {
  $$ = std::string{};
}
| octal_digit_sequence OCTAL_DIGIT {
  $$ = std::move($1);
  $$ += $2;
}
| octal_digit_sequence SINGLE_QUOTE OCTAL_DIGIT {
  $$ = std::move($1);
  $$ += $3;
}
;

decimal_literal
: decimal_digit_sequence
;

decimal_digit_sequence
: NONZERO_DIGIT {
  $$ = std::string{};
  $$ += $1;
}
| decimal_digit_sequence DIGIT {
  $$ = std::move($1);
  $$ += $2;
}
| decimal_digit_sequence SINGLE_QUOTE DIGIT {
  $$ = std::move($1);
  $$ += $3;
}
;

hexadecimal_literal
: HEXADECIMAL_PREFIX hexadecimal_digit_sequence {
  $$ = std::move($2);
}
;

hexadecimal_digit_sequence
: HEXADECIMAL_DIGIT {
  $$ = std::string{};
  $$ += $1;
}
| hexadecimal_digit_sequence HEXADECIMAL_DIGIT {
  $$ = std::move($1);
  $$ += $2;
}
| hexadecimal_digit_sequence SINGLE_QUOTE HEXADECIMAL_DIGIT {
  $$ = std::move($1);
  $$ += $3;
}
;

identification_expression
: namespace_name {
  $$ = std::make_unique<pache::var_exp>(std::move($1));
}

call_expression:
  primary_expression {
    $$ = std::move($1);
  }
| call_expression LEFT_PARENTHESIS FuncRParams RIGHT_PARENTHESIS {
  $$ = std::make_unique<pache::func_call_exp>(std::move($1), std::move($3));
}
| call_expression LEFT_SQUARE_BRACKET FuncRParams RIGHT_SQUARE_BRACKET {
  $$ = std::make_unique<pache::subscript_exp>(std::move($1), std::move($3));
}
| call_expression DOT IDENTIFIER {
  $$ = std::make_unique<pache::dot_exp>(std::move($1), std::move($3));
}
| call_expression ARROW IDENTIFIER  {
  $$ = std::make_unique<pache::arrow_exp>(std::move($1), std::move($3));
}
;

unary_expression
: call_expression {
    $$ = std::move($1);
}
| PLUS unary_expression {
    $$ = std::make_unique<pache::unary_plus>(std::move($2));
}
| MINUS unary_expression {
    $$ = std::make_unique<pache::unary_minus>(std::move($2));
}
| LOGICAL_NOT unary_expression {
  $$ = std::make_unique<pache::logical_not_exp>(std::move($2));
}
| BITWISE_NOT unary_expression {
  $$ = std::make_unique<pache::bitwise_not_exp>(std::move($2));
}
| UNARY_STAR unary_expression {
  $$ = std::make_unique<pache::indirection_exp>(std::move($2));
}
| B_AND unary_expression {
  $$ = std::make_unique<pache::address_of_exp>(std::move($2));
}
| ALLOCATION WHITESPACE type WHITESPACE unary_expression {
  $$ = std::make_unique<pache::allocation_exp>(std::move($3), std::move($5));
}
| DEALLOCATION WHITESPACE unary_expression {
  $$ = std::make_unique<pache::deallocation_exp>(std::move($3));
} 
;

mul_expressions:
  unary_expression {
    $$ = std::move($1);
  }
| mul_expressions  BINARY_STAR  unary_expression {
    $$ = std::make_unique<pache::binary_mul_exp>(std::move($1), std::move($3));
  }
| mul_expressions  SLASH  unary_expression {
    $$ = std::make_unique<pache::binary_div_exp>(std::move($1), std::move($3));
  }
| mul_expressions  PERCENT  unary_expression {
    $$ = std::make_unique<pache::binary_mod_exp>(std::move($1), std::move($3));
  }
;

add_exp:
  mul_expressions {
    $$ = std::move($1);
  }
| add_exp  BINARY_PLUS  mul_expressions {
    $$ = std::make_unique<pache::binary_plus_exp>(std::move($1), std::move($3));
  }
| add_exp BINARY_MINUS  mul_expressions {
    $$ = std::make_unique<pache::binary_minus_exp>(std::move($1), std::move($3));
  }
;

shift_exp
: add_exp {
  $$ = std::move($1);
}
| shift_exp  LEFT_SHITF  add_exp {
  $$ = std::make_unique<pache::left_shift_exp>(std::move($1), std::move($3));
}
| shift_exp  RIGHT_SHIFT  add_exp {
  $$ = std::make_unique<pache::right_shift_exp>(std::move($1), std::move($3));
}
;

three_way_expression:
  shift_exp {
    $$ = std::move($1);
  }
| three_way_expression  THREE_WAY_COMPARISON  add_exp {
    $$ = std::make_unique<pache::three_way_exp>(std::move($1), std::move($3));
}
;

rel_expression:
  three_way_expression {
    $$ = std::move($1);
  }
| rel_expression  LESS  three_way_expression {
    $$ = std::make_unique<pache::less_exp>(std::move($1), std::move($3));
}
| rel_expression  LESS_EQUAL  three_way_expression {
  $$ = std::make_unique<pache::less_eq_exp>(std::move($1), std::move($3));
}
| rel_expression  GREATER  three_way_expression {
  $$ = std::make_unique<pache::greater_exp>(std::move($1), std::move($3));
}
| rel_expression  GREATER_EQUAL  three_way_expression {
  $$ = std::make_unique<pache::greater_eq_exp>(std::move($1), std::move($3));
}
;

eq_expression:
  rel_expression {
    $$ = std::move($1);
  }
| eq_expression  EQUAL  rel_expression {
  $$ = std::make_unique<pache::eq_exp>(std::move($1), std::move($3));
}
| eq_expression  NOT_EQUAL  rel_expression {
  $$ = std::make_unique<pache::not_eq_exp>(std::move($1), std::move($3));
}
;

bitwise_and_expression:
  eq_expression {
    $$ = std::move($1);
  }
| bitwise_and_expression  B_AND  eq_expression {
  $$ = std::make_unique<pache::bitwise_and_exp>(std::move($1), std::move($3));
};

bitwise_xor_expression:
  bitwise_and_expression {
    $$ = std::move($1);
  }
| bitwise_xor_expression  B_XOR  bitwise_and_expression {
  $$ = std::make_unique<pache::bitwise_xor_exp>(std::move($1), std::move($3));
}
;

bitwise_or_expression:
  bitwise_xor_expression {
    $$ = std::move($1);
  }
| bitwise_or_expression  B_OR  bitwise_xor_expression {
  $$ = std::make_unique<pache::bitwise_or_exp>(std::move($1), std::move($3));
}
;

logical_and_expression:
  bitwise_or_expression {
    $$ = std::move($1);
  }
| logical_and_expression  AND  bitwise_or_expression {
  $$ = std::make_unique<pache::logical_and_exp>(std::move($1), std::move($3));
}
;

logical_or_expression:
  logical_and_expression {
    $$ = std::move($1);
  }
| logical_or_expression  OR  logical_and_expression {
  $$ = std::make_unique<pache::logical_or_exp>(std::move($1), std::move($3));
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
