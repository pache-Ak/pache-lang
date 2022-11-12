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
  pache::type_ast *type_val;
}

// lexer 返回的所有 token 种类的声明
// 注意 IDENT 和 INT_CONST 会返回 token 的值, 分别对应 str_val 和 int_val
%token I32 RETURN FUNC
%token <str_val> identifier
%token <int_val> INT_CONST

%token
                   // \n
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


  UNARY_STAR "unary *"
  PREFIX_STAR "prefix *"
  POSTFIX_STAR "postfix *"
  BINARY_STAR "binary *"

// 非终结符的类型定义
%type <ast_val> FuncDef main_func
%type <type_val> type
%type <exp_val> Number   primary_expression unary_expression
%type <stmt_val> stmt block return_stmt let_stmt
%type <exp_val>  expression  add_exp
%type <stmt_p_list> statement_list
%type <exp_val>  mul_expressions
%type <exp_val> three_way_expression rel_expression eq_expression logical_and_expression logical_or_expression

%%

// 开始符, CompUnit ::= FuncDef, 大括号后声明了解析完成后 parser 要做的事情
// 之前我们定义了 FuncDef 会返回一个 str_val, 也就是字符串指针
// 而 parser 一旦解析完 CompUnit, 就说明所有的 token 都被解析了, 即解析结束了
// 此时我们应该把 FuncDef 返回的结果收集起来, 作为 AST 传给调用 parser 的函数
// $1 指代规则里第一个符号的返回值, 也就是 FuncDef 的返回值
CompUnit
  : main_func {
    auto comp_unit = make_unique<pache::compunit_ast>();
    comp_unit->func_def = unique_ptr<pache::base_ast>($1);
    comp_unit->func_def->set_father(comp_unit.get());
    ast = move(comp_unit);
  };

main_func:
  FUNC MAIN LEFT_PARENTHESIS RIGHT_PARENTHESIS I32 block {
    auto ast = new pache::main_func_ast();
    ast->block = unique_ptr<pache::base_ast>($6);
    $$ = ast;
  }
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
  : FUNC identifier LEFT_PARENTHESIS RIGHT_PARENTHESIS type block {
    auto ast = new pache::func_ast();
    ast->return_type = unique_ptr<pache::base_ast>($5);
    ast->ident = *unique_ptr<string>($2);
    ast->block = unique_ptr<pache::base_ast>($6);
    $$ = ast;
  };

// 同上, 不再解释
type
  : I32 {
    $$ = new pache::type_ast("i32");
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
  return_stmt
    { $$ = $1; }
| block
    { $$ = $1; }
| let_stmt {
    $$ = $1;
}
;



return_stmt
  : RETURN expression  {
    auto ast =  new pache::return_ast($2);
    $$ = ast;
  }
  ;

let_stmt:
  LET type identifier  {
    auto var = new pache::variable_ast($2, $3);
    $$ = new pache::let_stmt(var);
  }

Number
  : INT_CONST {
    $$ = new pache::i32_literal($1);
  }
  ;

expression:
  logical_or_expression {
    $$ = $1;
  }
;

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
