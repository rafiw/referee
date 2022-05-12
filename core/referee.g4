grammar referee;

program     : (statement ';')*
            ;

statement   : declaraion
            | expression
            ;

declaraion  : declType
            | declData
            ;

sign        : '+'
            | '-'
            ;

BOOLEAN     : 'true'
            | 'false'
            ;
boolean     : BOOLEAN;

INTEGER     : [1-9][0-9]* ;
integer     : INTEGER;

FLOATING    : [1-9][0-9]*'.'[0-9]* 
            | '0'?'.'[0-9]+
            ;
EXPONENT    : [eE][+-]?[1-9][0-9]*;
floating    : FLOATING EXPONENT?
            | INTEGER  EXPONENT
            ;

STRING      : '"' [a-zA-Z_0-9] '"' ;         
string      : STRING;

ID          : [a-zA-Z_][a-zA-Z0-9]*
            ;

WHITESPACE  : [ \t\n]+ -> skip ;

time        : '[' integer ':' integer ']'
            | '['         ':' integer ']'
            | '[' integer ':'         ']'
            ;

expression  : sign? integer                                     # ExprConst
            | sign? floating                                    # ExprConst
            | string                                            # ExprConst
            | boolean                                           # ExprConst

            | reference                                         # ExprRef

            | expression '=='  expression                       # ExprEq
            | expression '!='  expression                       # ExprNe

            | expression '<'   expression                       # ExprLt
            | expression '<='  expression                       # ExprLe

            | expression '>'   expression                       # ExprGt
            | expression '>='  expression                       # ExprGe

            | expression '+'   expression                       # ExprAdd
            | expression '-'   expression                       # ExprSub
            | expression '*'   expression                       # ExprMul
            | expression '/'   expression                       # ExprDiv

            | expression '||'  expression                       # ExprOr
            | expression '&&'  expression                       # ExprAnd
            | expression '^'   expression                       # ExprXor
            | expression '=>'  expression                       # ExprImp
            | expression '<=>' expression                       # ExprEqu

            | 'G'  time? '(' expression ')'                     # ExprG
            | 'F'  time? '(' expression ')'                     # ExprF
            | 'Xs' time? '(' expression ')'                     # ExprXs
            | 'Xw' time? '(' expression ')'                     # ExprXw
            | 'Us' time? '(' expression ',' expression ')'      # ExprUs
            | 'Uw' time? '(' expression ',' expression ')'      # ExprUw
            | 'Rs' time? '(' expression ',' expression ')'      # ExprRs
            | 'Rw' time? '(' expression ',' expression ')'      # ExprRw

            | 'H'  time? '(' expression ')'                     # ExprH
            | 'O'  time? '(' expression ')'                     # ExprO
            | 'Ys' time? '(' expression ')'                     # ExprYs
            | 'Yw' time? '(' expression ')'                     # ExprYw
            | 'Ss' time? '(' expression ',' expression ')'      # ExprSs
            | 'Sw' time? '(' expression ',' expression ')'      # ExprSw
            | 'Ts' time? '(' expression ',' expression ')'      # ExprTs
            | 'Tw' time? '(' expression ',' expression ')'      # ExprTw

            | 'I'  time? '(' expression ',' expression ')'      # ExprInt
            | 'I'  time? '(' expression ')'                     # ExprInt

            | '(' expression ')'                                # ExprParen
            ;

reference   : dataID                                            # RefData
            | reference '.' ID                                  # RefMmbr
            | reference '[' expression ']'                      # RefElem
            ;

typeID      : ID
            ;

dataID      : ID
            ;


itemList    : (ID (',' ID)*)?
            ;

mmbrList    : (ID ':' typeID ';')*
            ;

index       : integer
            ;

type        : 'boolean'
            | 'integer'
            | 'floating'
            | 'string'
            | 'struct' '{' mmbrList '}'
            | 'enum'   '{' itemList '}'
            | typeID  ('[' index    ']')*
            ;

declType    : 'type' ID ':' type
            ;

declData    : 'data' ID ':' type
            ;
