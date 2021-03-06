grammar EGrammar;
import ELex;



start:  NEWLINE* (stat? NEWLINE)* EOF;

stat:   func                                                                                # defFunc
    |   line                                                                                # none
    |   (ID (','ID)*) '=' SPEC_SYM='param' '(' ( STRINGLITERAL (','STRINGLITERAL)*)? ')'    # assignParam
    |   SPEC_SYM='out'  (ID (','ID)*)                                                       # outParam
    ;

func    : ('func'|'function') ID '('args ')' ':'?  NEWLINE* ( stat NEWLINE*)+ SPEC_SYM=(';'|'end')
    ;

//convolve: 'convolve' '('expr ',' expr (','STRINGLITERAL )? ')' ;
//
line :   ID '=' expr                                     # assign
     |   SPEC_SYM='return ' (expr (','expr)*)            # assignRetParam
     ;

args:  (ID (','ID)*)  ;

unaryBInFunc : LOG|LOG10|COS|SIN|EXP;
tConvBInFunc: C_FLOAT|C_INT|C_DOUBLE|C_INT64;

//cond: expr;
left_expr: expr;
right_expr: expr ;


expr:   ID '(' (expr (','expr)*) ')'                                    # callFunc
    |   SPEC_SYM='integrate'  '('expr')'                                # callIntegrate
    |   SPEC_SYM='convolve'  '('expr','expr(','STRINGLITERAL)?')'       # callConvolve
    |   SPEC_SYM='recconvolve'  '('expr','expr(','STRINGLITERAL)?')'    # callRecConvolve
    |   SPEC_SYM='range'     '('(expr (','expr)*)')'                    # range
    |   SPEC_SYM='decimation''('expr ','expr')'                         # decimation
    |   SPEC_SYM='shift'     '('expr ','expr')'                         # shift
    |   ID'[' expr ']'                                                  # neighborhoodPoint
    |   unaryBInFunc '(' expr ')'                                       # callUnaryBInFunc
    |   tConvBInFunc '(' expr ')'                                       # callTConvBInFunc
    |   ID '.' 'type' '(' expr ')'                                      # callLinkedTypeCast
    |   expr ('**'|'^')   expr                                          # pow
    |   '-' expr                                                        # inv
    |   expr op=('*'|'/') expr                                          # mulDiv
    |   expr op=('+'|'-') expr                                          # addSub
    |   expr op=('<<'|'>>') expr                                        # bitwiseShift
    |   expr op=('>'|'<') expr                                          # moreLess
    |   expr op=('>='|'<=') expr                                        # moreeqLesseq
    |   expr op=('=='|'!=') expr                                        # equality
    |   expr  '&' expr                                                  # bitwiseAnd
    |   expr  '|' expr                                                  # bitwiseOr
    |   expr  'bxor' expr                                               # bitwiseXor
    |   expr '?' left_expr ':' right_expr                               # condExpr
    |   (DOUBLE|FLOAT|INT64|INT)                                        # const
    |   ID                                                              # id
    |   '(' expr ')'                                                    # parens
    |   '[' (expr (','expr)*) ']'                                       # smallArrayDefinition
    ;
