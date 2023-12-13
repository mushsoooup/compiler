# compiler

Simple compiler for Sysy2022, a subset of the c language.

### Grammar

##### Expressions

AddExpr -> MulExpr { ('+' | '-') MulExpr }

MulExpr -> UnaryExpr { ('*' | '/' | '%') UnaryExpr }

UnaryExpr → Callee | UnaryOp | '(' AddExpr ')' | LVal | Number

UnaryOp -> ('+' | '−' | '!') UnaryExpr

LVal → IDENT {'[' AddExpr ']'}

Number → INT | FLOAT

Callee -> IDENT '(' AddExpr { ',' AddExpr } ')'

##### Relational Exprressions

RelExpr -> AddExpr { ('<' | '>' | '<=' | '>=') AddExpr }

EqExpr → RelExpr { ('==' | '!=') RelExpr }

LAndExpr → EqExpr { '&&' EqExpr }

LOrExpr → LAndExpr { '||' LAndExpr }

##### Definitions

CompUnit -> { (Decl | Funcdef) }

Decl -> ConstDecl | VarDecl

ConstDecl -> 'const' Type ConstDef { ',' ConstDef } ';'

ConstDef -> IDENT { '[' AddExpr ']' } '=' InitVal // InitVal evaluation at compile time

InitVal → AddExpr | Array

Array -> '{' [ InitVal { ',' InitVal } ] '}'

VarDecl → Type VarDef { ',' VarDef } ';'

VarDef → IDENT { '[' AddExpr ']' } ['=' InitVal] // AddExpr evaluation at compile time

FuncDef → Type IDENT '(' [FuncFParam] { ',' FuncFParam } ')' Block

FuncFParam → Type IDENT ['[' ']' { '[' AddExpr ']' }]

Type → 'void' | 'int' | 'float'

##### Statements

Block → '{' { BlockItem } '}'

BlockItem → Decl | Stmt

Stmt → AssignStmt | EvalStmt | Block | IfStmt | WhileStmt | BreakStmt | ContinueStmt | ReturnStmt

AssignStmt -> LVal '=' AddExpr ';'

EvalStmt -> [AddExpr] ';'

IfStmt -> 'if' '( LOrExpr ')' Stmt [ 'else' Stmt ]

WhileStmt -> 'while' '(' LOrExpr ')' Stmt

BreakStmt -> 'break' ';'

ContinueStmt -> 'continue' ';'

ReturnStmt -> 'return' [AddExpr] ';'

