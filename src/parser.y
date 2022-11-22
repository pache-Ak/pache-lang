%code requires {
  #include <memory>
  #include <string>
  #include "../ast/ast.h"
  #include "../ast/expression.h"
  #include "../ast/statement.h"
}

%{

#include <iostream>
#include <memory>
#include <string>
#include "../ast/ast.h"
#include "../ast/expression.h"
#include "../ast/statement.h"
#include "../ast/type.h"
#include "../ast/literal.h"

// 声明 lexer 函数和错误处理函数
int yylex();
void yyerror(std::unique_ptr<pache::base_ast> &ast , const char *s);

using namespace std;

%}

// 定义 parser 函数和错误处理函数的附加参数
// 我们需要返回一个字符串作为 AST, 所以我们把附加参数定义成字符串的智能指针
// 解析完成后, 我们要手动修改这个参数, 把它设置成解析得到的字符串
%parse-param { std::unique_ptr<pache::base_ast> &ast }

// yylval 的定义, 我们把它定义成了一个联合体 (union)
// 因为 token 的值有的是字符串指针, 有的是整数
// 之前我们在 lexer 中用到的 str_val 和 int_val 就是在这里被定义的
// 至于为什么要用字符串指针而不直接用 string 或者 unique_ptr<string>?
// 请自行 STFW 在 union 里写一个带析构函数的类会出现什么情况
%union {
  std::string *str_val;
  int int_val;
  pache::base_ast *ast_val;
  pache::exp_ast *exp_val;
  std::vector<pache::stmt_ast*> *stmt_p_list;
  pache::stmt_ast *stmt_val;
  pache::block_ast *block_val;
  pache::type_ast const*type_val;
  pache::func_arg *FuncFParam;
  std::vector<pache::func_arg*> *FuncFParam_p_list;
  std::vector<pache::exp_ast*> *FuncRParam_p_list;
  pache::variable_ast *var_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val

%token
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
  UNARY_STAR "unary *"
  PREFIX_STAR "prefix *"
  POSTFIX_STAR "postfix *"
  BINARY_STAR "binary *"
%token I32 RETURN FUNC
%token <str_val> identifier
%token <int_val> INT_CONST

// 非终结符的类型定义
%type <ast_val>   CompUnit
%type <var_val> FuncDef main_func
%type <type_val> type
%type <exp_val> Number   primary_expression unary_expression
%type <block_val> block optional_else if_stmt loop_stmt
%type <stmt_val> stmt return_stmt let_stmt assign_stmt break_stmt continue_stmt
%type <exp_val>  expression  add_exp
%type <stmt_p_list> statement_list
%type <exp_val>  mul_expressions
%type <exp_val> three_way_expression rel_expression eq_expression logical_and_expression logical_or_expression
%type <FuncFParam> FuncFParam
%type <FuncFParam_p_list> FuncFParams
%type <FuncRParam_p_list> FuncRParams
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
| CompUnit FuncDef {
      ast->insert_dec($2);
};

FuncFParam:
  type identifier {
    $$ = new pache::func_arg($1, $2);
  };

FuncFParams:
  // empty

  { $$ = new std::vector<pache::func_arg*>();
  }
  | FuncFParam {
    $$ = new std::vector<pache::func_arg*>();
    $$->push_back($1);
  }
| FuncFParams COMMA FuncFParam {
      $$ = $1;
      $$->push_back($3);

};
main_func:
  FUNC MAIN LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS I32 block {
    auto ast = new pache::main_func_ast(pache::i32_type_t::get_i32_type(), new string("main"), $4);
    ast->block = unique_ptr<pache::base_ast>($7);
     // for (auto arg : *$4) {
     //   arg->set_father($$);
     // }
    $$ = ast;
  };
// FuncDef ::= type IDENT LEFT_PARENTHESIS RIGHT_PARENTHESIS block;
// 我们这里可以直接写 LEFT_PARENTHESIS 和 RIGHT_PARENTHESIS, 因为之前在 lexer 里已经处理了单个字符的情况
// 解析完成后, 把这些符号的结果收集起来, 然后拼成一个新的字符串, 作为结果返回
// $$ 表示非终结符的返回值, 我们可以通过给这个符号赋值的方法来返回结果
// 你可能会问, type, IDENT 之类的结果已经是字符串指针了
// 为什么还要用 unique_ptr 接住它们, 然后再解引用, 把它们拼成另一个字符串指针呢
// 因为所有的字符串指针都是我们 new 出来的, new 出来的内存一定要 delete
// 否则会发生内存泄漏, 而 unique_ptr 这种智能指针可以自动帮我们 delete
// 虽然此处你看不出用 unique_ptr 和手动 delete 的区别, 但当我们定义了 AST 之后
// 这种写法会省下很多内存管理的负担
FuncDef
  : FUNC identifier LEFT_PARENTHESIS FuncFParams RIGHT_PARENTHESIS type block {
    auto ast = new pache::func_ast($6, $2, $4);
    ast->block = unique_ptr<pache::base_ast>($7);
    //      for (auto arg : *$4) {
     //   arg->set_father($$);
     // }
    $$ = ast;
  }
| main_func {
  $$ = $1;
};

// 同上, 不再解释
type
  : I32 {
    $$ = pache::i32_type_t::get_i32_type();
  }
  ;


statement_list:
  // Empty
    { $$ = new std::vector<pache::stmt_ast*>(); }
| statement_list stmt
    {
      $$ = $1;;
      $$->push_back($2);
    }
;
block
  : LEFT_CURLY_BRACE statement_list RIGHT_CURLY_BRACE {
    $$ = new pache::block_ast($2);
    for (auto & ast : *$2) {
      ast->set_father($$);
    }
  }
  ;

stmt:
  assign_stmt {
  $$ = $1;
}
| expression {
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
  RETURN {
    $$ = new pache::return_ast(&pache::void_literal);
  }
| RETURN expression  {
    $$ = new pache::return_ast($2);
  }
  ;

let_stmt:
  LET type identifier ASSIGN expression {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var, $5);
  }
| LET type identifier LEFT_CURLY_BRACE expression RIGHT_CURLY_BRACE {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var, $5);
}
| LET type identifier  {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var, nullptr);
  }
;

assign_stmt:
  identifier ASSIGN expression {
    auto exp = new pache::var_exp($1);
    $$ = new pache::assign_stmt(exp, $3);
  }
;

break_stmt:
  BREAK {
    $$ = new pache::break_stmt();
  };

continue_stmt:
  CONTINUE {
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
  : INT_CONST {
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
    $$ = new std::vector<pache::exp_ast*>();
  }|
  expression {
    $$ = new std::vector<pache::exp_ast*>{};
    $$->push_back($1);
  }
| FuncRParams COMMA expression {
  $$ = $1;
  $$->push_back($3);
};

primary_expression:
  LEFT_PARENTHESIS expression RIGHT_PARENTHESIS {
    $$ = $2;
  }
| Number {
    $$ = $1;
  }
| identifier {
    $$ = new pache::var_exp($1);
}
;

unary_expression:
  primary_expression {
    $$ = $1;
  }
| identifier LEFT_PARENTHESIS FuncRParams RIGHT_PARENTHESIS {
  $$ = new pache::func_call_exp($1, $3);
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

logical_and_expression:
  eq_expression {
    $$ = $1;
  }
| logical_and_expression AND eq_expression {
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
void yyerror(std::unique_ptr<pache::base_ast> &ast, const char *s) {
  cerr << "error: " << s << endl;
}
