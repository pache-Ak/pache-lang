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

MAIN                 "main"
LET                  "let"

IF                   "if"
ELSE                 "else"

LOOP                 "loop"
BREAK                "break"
CONTINUE             "continue"

ASSIGN               ":="

LEFT_CURLY_BRACE     "{"
LEFT_PARENTHESIS     "("
LEFT_SQUARE_BRACKET  "["
PLUS                 "+"
MINUS                "-"
PERCENT              "%"
SLASH                "/"
PERIOD               "."
RIGHT_CURLY_BRACE    "}"
RIGHT_PARENTHESIS    ")"
RIGHT_SQUARE_BRACKET "]"

THREE_WAY_COMPARISON "<=>"
LESS                 "<"
LESS_EQUAL           "<="
GREATER              ">"
GREATER_EQUAL        ">="
EQUAL                "=="
NOT_EQUAL            "!="

AND                  "&&"
OR                   "||"


/* 空白符和注释 */
horizontal_whitespace [ \t\r]
whitespace            [ \t\r\n]
one_line_comment      \/\/[^\n]*\n


/* 标识符 */
identifier    [a-zA-Z_][a-zA-Z0-9_]*
operand_start         [(A-Za-z0-9_\"]
/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+
Binary        0[bB][01]*

%%

{MAIN}          { return MAIN; }
{LET}           { return LET; }

{IF}            { return IF; }
{ELSE}          { return ELSE; }

{ASSIGN}        { return ASSIGN; }
{one_line_comment}   { /* 忽略, 不做任何操作 */ }
{horizontal_whitespace}+ {
  // Make the span empty by setting start to end.
  //context.current_token_position.step();
  BEGIN(AFTER_WHITESPACE);
}

\n+ {
  // Advance end by yyleng lines, resetting the column to zero.
  //context.current_token_position.lines(yyleng);
  // Make the span empty by setting start to end.
  //context.current_token_position.step();
  BEGIN(AFTER_WHITESPACE);
}
"i32"           { return I32; }
"return"        { return RETURN; }
"func"          { return FUNC; }

{LEFT_CURLY_BRACE}      { return LEFT_CURLY_BRACE; }
{LEFT_PARENTHESIS}      { return LEFT_PARENTHESIS; }
{LEFT_SQUARE_BRACKET}   { return LEFT_SQUARE_BRACKET; }
{PLUS}                  { return PLUS; }
{MINUS}                 { return MINUS; }
{PERCENT}               { return PERCENT; }
{SLASH}                 { return SLASH; }
{PERIOD}                { return PERIOD; }

{THREE_WAY_COMPARISON}  { return THREE_WAY_COMPARISON; }
{LESS}                  { return LESS; }
{LESS_EQUAL}            { return LESS_EQUAL; }
{GREATER}               { return GREATER; }
{GREATER_EQUAL}         { return GREATER_EQUAL; }
{EQUAL}                 { return EQUAL; }
{NOT_EQUAL}             { return NOT_EQUAL; }

{AND}                   { return AND; }
{OR}                    { return OR; }

{LOOP}                  { return LOOP; }
{BREAK}                 { return BREAK; }
{CONTINUE}              { return CONTINUE; }

{identifier}    { yylval.str_val = new string(yytext); return identifier; }

{Decimal}       { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Octal}         { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Hexadecimal}   { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }
{Binary}        { yylval.int_val = strtol(yytext, nullptr, 0); return INT_CONST; }


{RIGHT_PARENTHESIS} {
  BEGIN(AFTER_OPERAND);
  return RIGHT_PARENTHESIS;
}
{RIGHT_CURLY_BRACE} {
  BEGIN(AFTER_OPERAND);
  return RIGHT_CURLY_BRACE;
}
{RIGHT_SQUARE_BRACKET} {
  BEGIN(AFTER_OPERAND);
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
