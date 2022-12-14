%{ /* -*- C++ -*- */
#include <cstdlib>
#include <string>

// 因为 Flex 会用到 Bison 中关于 token 的定义
// 所以需要 include Bison 生成的头文件
#include "../build/parser.hpp"
#include "../src/driver.h"
using namespace std;

%}

%option noyywrap nounput noinput batch debug

/* Lexing a token immediately after consuming some whitespace. */
%s AFTER_WHITESPACE
/*
 * Lexing a token immediately after consuming an operand-ending token:
 * a closing bracket, IDENTIFIER, or literal.
 */
%s AFTER_OPERAND

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

MAIN                 "main"
LET                  "let"
CLASS                "class"
IF                   "if"
ELSE                 "else"

LOOP                 "loop"
BREAK                "break"
CONTINUE             "continue"
SEMICOLON            ";"
ASSIGN               ":="
B_AND                "&"
B_XOR                "^"
B_OR                 "|"
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

COMMA                ","

/* 空白符和注释 */
horizontal_whitespace [ \t\r]
whitespace            [ \t\r\n]

one_line_comment      \/\/[^\n]*\n

/* 标识符 */
IDENTIFIER    [a-zA-Z_][a-zA-Z0-9_]*
operand_start         [(A-Za-z0-9_\"]
/* 整数字面量 */
Decimal       [1-9][0-9]*
Octal         0[0-7]*
Hexadecimal   0[xX][0-9a-fA-F]+
Binary        0[bB][01]*
%{
  // Code run each time a pattern is matched.
  # define YY_USER_ACTION  loc.columns (yyleng);
%}
%%
%{
  // A handy shortcut to the location held by the driver.
  pache::location loc;
  // Code run each time yylex is called.
  loc.step ();
%}
{MAIN}          { return pache::parser::make_MAIN(loc); }
{LET}           { return pache::parser::make_LET(loc); }
{B_AND}         { return pache::parser::make_B_AND(loc); }
{B_XOR}         { return pache::parser::make_B_XOR(loc); }
{B_OR}         { return pache::parser::make_B_OR(loc); }
{IF}            { return pache::parser::make_IF(loc); }
{ELSE}          { return pache::parser::make_ELSE(loc); }

{ASSIGN}        { return pache::parser::make_ASSIGN(loc); }
{one_line_comment}   { /* 忽略, 不做任何操作 */ }
{horizontal_whitespace}+ {
  // Make the span empty by setting start to end.
  //context.current_token_position.step();
  BEGIN(AFTER_WHITESPACE);
}

{VOID}          { return pache::parser::make_VOID(loc); }

{BOOL}          { return pache::parser::make_BOOL(loc); }

{SIZE}          { return pache::parser::make_SIZE(loc); }

{I8}            { return pache::parser::make_I8(loc); }
{I16}           { return pache::parser::make_I16(loc); }
{I32}           { return pache::parser::make_I32(loc); }
{I64}           { return pache::parser::make_I64(loc); }
{I128}          { return pache::parser::make_I128(loc); }

{U8}            { return pache::parser::make_U8(loc); }
{U16}           { return pache::parser::make_U16(loc); }
{U32}           { return pache::parser::make_U32(loc); }
{U64}           { return pache::parser::make_U64(loc); }
{U128}          { return pache::parser::make_U128(loc); }

{F16}           { return pache::parser::make_F16(loc); }
{F32}           { return pache::parser::make_F32(loc); }
{F64}           { return pache::parser::make_F64(loc); }
{F128}          { return pache::parser::make_F128(loc); }

{D32}           { return pache::parser::make_D32(loc); }
{D64}           { return pache::parser::make_D64(loc); }

{C8}            { return pache::parser::make_C8(loc); }
{C16}           { return pache::parser::make_C16(loc); }
{C32}           { return pache::parser::make_C32(loc); }

"return"        { return pache::parser::make_RETURN(loc); }
"func"          { return pache::parser::make_FUNC(loc); }
{CLASS}                 { return pache::parser::make_CLASS(loc); }


{LEFT_CURLY_BRACE}      { return pache::parser::make_LEFT_CURLY_BRACE(loc); }
{LEFT_PARENTHESIS}      { return pache::parser::make_LEFT_PARENTHESIS(loc); }
{LEFT_SQUARE_BRACKET}   { return pache::parser::make_LEFT_SQUARE_BRACKET(loc); }
{PLUS}                  { return pache::parser::make_PLUS(loc); }
{MINUS}                 { return pache::parser::make_MINUS(loc); }
{PERCENT}               { return pache::parser::make_PERCENT(loc); }
{SLASH}                 { return pache::parser::make_SLASH(loc); }
{PERIOD}                { return pache::parser::make_PERIOD(loc); }

{THREE_WAY_COMPARISON}  { return pache::parser::make_THREE_WAY_COMPARISON(loc); }
{LESS}                  { return pache::parser::make_LESS(loc); }
{LESS_EQUAL}            { return pache::parser::make_LESS_EQUAL(loc); }
{GREATER}               { return pache::parser::make_GREATER(loc); }
{GREATER_EQUAL}         { return pache::parser::make_GREATER_EQUAL(loc); }
{EQUAL}                 { return pache::parser::make_EQUAL(loc); }
{NOT_EQUAL}             { return pache::parser::make_NOT_EQUAL(loc); }

{AND}                   { return pache::parser::make_AND(loc); }
{OR}                    { return pache::parser::make_OR(loc); }

{LOOP}                  { return pache::parser::make_LOOP(loc); }
{BREAK}                 { return pache::parser::make_BREAK(loc); }
{CONTINUE}              { return pache::parser::make_CONTINUE(loc); }



{COMMA}                 { return pache::parser::make_COMMA(loc); }
{SEMICOLON}           { return pache::parser::make_SEMICOLON(loc); }
{IDENTIFIER}   { return pache::parser::make_IDENTIFIER(yytext, loc);}

{Decimal}       {return pache::parser::make_INTEGER (std::strtol(yytext, nullptr, 0), loc);}
{Octal}         return pache::parser::make_INTEGER (std::strtol(yytext, nullptr, 0), loc);
{Hexadecimal}   return pache::parser::make_INTEGER (std::strtol(yytext, nullptr, 0), loc);
{Binary}        return pache::parser::make_INTEGER (std::strtol(yytext, nullptr, 0), loc);

\n+    {   // Advance end by yyleng lines, resetting the column to zero.
  //context.current_token_position.lines(yyleng);
  // Make the span empty by setting start to end.
  //context.current_token_position.step();
  BEGIN(AFTER_WHITESPACE);
   }
{RIGHT_PARENTHESIS} {
  BEGIN(AFTER_OPERAND);
  return pache::parser::make_RIGHT_PARENTHESIS(loc);
}
{RIGHT_CURLY_BRACE} {
  BEGIN(AFTER_OPERAND);
  return pache::parser::make_RIGHT_CURLY_BRACE(loc);
}
{RIGHT_SQUARE_BRACKET} {
  BEGIN(AFTER_OPERAND);
  return pache::parser::make_RIGHT_SQUARE_BRACKET(loc);
}

 /* `*` operator case 1: */
<AFTER_WHITESPACE>"*"{whitespace}+ {
  BEGIN(AFTER_WHITESPACE);
  return pache::parser::make_BINARY_STAR(loc);
}
 /* `*` operator case 2: */
{horizontal_whitespace}"*"{horizontal_whitespace} { return pache::parser::make_BINARY_STAR(loc); }
 /* `*` operator case 3: */
<AFTER_WHITESPACE>"*" { return pache::parser::make_PREFIX_STAR(loc); }
 /* `*` operator case 4: */
<INITIAL,AFTER_OPERAND>"*"{whitespace}+ {
  BEGIN(AFTER_WHITESPACE);
  return pache::parser::make_POSTFIX_STAR(loc);
}
 /* `*` operator case 5: */
<INITIAL,AFTER_OPERAND>"*" { return pache::parser::make_UNARY_STAR(loc); }

<<EOF>>  return pache::parser::make_EOF (loc);
%%

void
pache::driver::scan_begin (FILE *f)
{
    yyin = f;

}

void
pache::driver::scan_end ()
{
  fclose (yyin);
}