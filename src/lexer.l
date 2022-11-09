%option noyywrap
%option nounput
%option noinput

%{

#include <cstdlib>
#include <string>

// 因为 Flex 会用到 Bison 中关于 token 的定义
// 所以需要 include Bison 生成的头文件
#include "../build/parser.hpp"

using namespace std;

%}

/* Lexing a token immediately after consuming some whitespace. */
%s AFTER_WHITESPACE
/*
 * Lexing a token immediately after consuming an operand-ending token:
 * a closing bracket, identifier, or literal.
 */
%s AFTER_OPERAND

LEFT_CURLY_BRACE     "{"
LEFT_PARENTHESIS     "("
LEFT_SQUARE_BRACKET  "["
PLUS  "+"
MINUS "-"
PERCENT              "%"
SLASH                "/"
PERIOD               "."
RIGHT_CURLY_BRACE    "}"
RIGHT_PARENTHESIS    ")"
RIGHT_SQUARE_BRACKET "]"


/* 空白符和注释 */
horizontal_whitespace [ \t\r]
whitespace            [ \t\r\n]
LineComment   "//".*

/* 标识符 */
identifier    [a-zA-Z_][a-zA-Z0-9_]*
operand_start         [(A-Za-z0-9_\"]
/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+
Binary        0[bB][01]*

%%

{LineComment}   { /* 忽略, 不做任何操作 */ }
{horizontal_whitespace}+ {
  ///   TODO:
}
{whitespace}    {  }

\n+ {
  /// TODO:
}

"i32"           { return I32; }
"return"        { return RETURN; }
"func"          { return FUNC; }
{identifier}    { yylval.str_val = new string(yytext); return identifier; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Binary}        { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }

{LEFT_CURLY_BRACE}    { return LEFT_CURLY_BRACE; }
{LEFT_PARENTHESIS}    { return LEFT_PARENTHESIS; }
{LEFT_SQUARE_BRACKET} { return LEFT_SQUARE_BRACKET; }
{PLUS}                { return PLUS; }
{MINUS}               { return MINUS; }
{PERCENT}             { return PERCENT; }
{SLASH}               { return SLASH; }
{PERIOD}              { return PERIOD; }


{RIGHT_PARENTHESIS} {
 // BEGIN(AFTER_OPERAND);
  return RIGHT_PARENTHESIS;
}
{RIGHT_CURLY_BRACE} {
 // BEGIN(AFTER_OPERAND);
  return RIGHT_CURLY_BRACE;
}
{RIGHT_SQUARE_BRACKET} {
 // BEGIN(AFTER_OPERAND);
  return RIGHT_SQUARE_BRACKET;
}

 /* `*` operator case 1: */
<AFTER_WHITESPACE>"*"{whitespace}+ {
  BEGIN(AFTER_WHITESPACE);
  return BINARY_STAR;
}
 /* `*` operator case 2: */
{horizontal_whitespace}"*"{horizontal_whitespace} { return BINARY_STAR; }
 /* `*` operator case 3: */
<AFTER_WHITESPACE>"*" { return PREFIX_STAR; }
 /* `*` operator case 4: */
<INITIAL,AFTER_OPERAND>"*"{whitespace}+ {
  BEGIN(AFTER_WHITESPACE);
  return POSTFIX_STAR;
}
 /* `*` operator case 5: */
<INITIAL,AFTER_OPERAND>"*" { return UNARY_STAR; }
%%
