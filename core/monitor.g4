grammar monitor;

program : (statement ';')*
        ;

statement
        : declaraion
        | expression
        ;

declaration
        : declType
        | declData
        ;

sign    : '+'
        | '-'
        ;

BOOLEAN : 'true'
        | 'false'
        ;
boolean : BOOLEAN;

INTEGER : [1-9][0-9]* ;
integer : INTEGER;

FLOATING: (     [1-9][0-9]*'.'[0-9]*
            |            0?'.'[0-9]+)([eE][1-9][0-9]*)?
floating: FLOATING;

STRING  : '"' [a-zA-Z_0-9] '"' ;         
string  : STRING;

expression
        : 

        | sign? integer
        | sign? floating
        | string
        | boolean

        | reference                     # ExprRef

        | expression '+' expression     # ExprAdd
        | expression '-' expression     # ExprSub
        | expression '*' expression     # ExprMul
        | expression '/' expression     # ExprDiv

        | 'G' expression                # ExprG
        | 'F' expression                # ExprF
        | 'Xs' expression               # ExprXs
        | 'Xw' expression               # ExprXw
        | expression 'Us' expression    # ExprUs
        | expression 'Uw' expression    # ExprUw
        | expression 'Rs' expression    # ExprRs
        | expression 'Rw' expression    # ExprRw

        | 'H' expression                # ExprH
        | 'O' expression                # ExprO
        | 'Ys' expression               # ExprYs
        | 'Yw' expression               # ExprYw
        | expression 'Ss' expression    # ExprSs
        | expression 'Sw' expression    # ExprSw
        | expression 'Ts' expression    # ExprTs
        | expression 'Tw' expression    # ExprTw

        | '(' expression ')'            # ExprParen
        ;

reference
        : dataID
        | reference '.' ID              # RefMmbr
        | reference '[' expression ']'  # RefElem
        ;

typeID  : ID
        ;

dataID  : ID
        ;


itemList: (ID (',' ID)*)?
        ;

mmbrList: (ID ':' typeID ';')*
        ;

type    : 'boolean'
        | 'integer'
        | 'floating'
        | 'string'
        | 'struct' '{' mmbrList '}'
        | 'enum'   '{' itemList '}'
        | typeID  ('[' index    ']')*
        ;

declType: type ID : type
        ;

delcData: data ID : type
        ;
