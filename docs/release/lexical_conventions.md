## Lexical conventions

---

### Preprocessing tokens
Preprocessing-token:

*import*

*module*

*export*

*identifier*

*pp-number*

*character-literal*

*user-defined-character-literal*

*string-literal*

*user-defined-string-literal*

*preprocessing-op-or-punc*

### tokens
token:

*identifier*

*keyword*

*literal*

*operator-or-punctuator*

###  Comments

### Preprocessing numbers
```BNF
pp-number:
    digit
    . digit
    pp-number digit
    pp-number identifier-nondigit
    pp-number ’ digit
    pp-number ’ nondigit
    pp-number e sign
    pp-number E sign
    pp-number p sign
    pp-number P sign
    pp-number .
```

### Identifiers
```BNF
identifier:
    identifier-nondigit
    identifier identifier-nondigit
    identifier digit

identifier-nondigit:
    nondigit
    universal-character-name

nondigit: one of
    a b c d e f g h i j k l m
    n o p q r s t u v w x y z
    A B C D E F G H I J K L M
    N O P Q R S T U V W X Y Z _

digit: one of
    0 1 2 3 4 5 6 7 8 9
```
### Literal
#### Kinds of  literals
```BNF
literal:
    integer-literal
    character-literal
    floating-point-literal
    string-literal
    boolean-literal
    pointer-literal
    user-defined-literal
```
#### Interger literals
```BNF
integer-literal:
    binary-literal integer-suffix
    octal-literal integer-suffix
    decimal-literal integer-suffix
    hexadecimal-literal integer-suffix

binary-literal:
    0b binary-digit
    0B binary-digit
    binary-literal 'opt binary-digit

octal-literal:
    0
    octal-literal 'opt octal-digit

decimal-literal:
    nonzero-digit
    decimal-literal 'opt digit

hexadecimal-literal:
    hexadecimal-prefix hexadecimal-digit-sequence

binary-digit: one of
    0 1

octal-digit: one of
    0 1 2 3 4 5 6 7

nonzero-digit: one of
    1 2 3 4 5 6 7 8 9

hexadecimal-prefix: one of
    0x 0X

hexadecimal-digit-sequence :
    hexadecimal-digit
    hexadecimal-digit-sequence 'opt hexadecimal-digit

hexadecimal-digit: one of
    0 1 2 3 4 5 6 7 8 9
    a b c d e f
    A B C D E F

integer-suffix: one of
    i u
    i8 i16 i32 i64 i128
    u8 u16 u32 u64 u128
    isize usize
```
### A
```lex
hex-quad:
    hexadecimal-digit hexadecimal-digit
    hexadecimal-digit hexadecimal-digit

universal-character-name :
    \u hex-quad
    \U hex-quad hex-quad
```

