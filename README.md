# Language

## The grammar of the language

```bnf

<Program> ::= <Statement>*

<Statement> ::=
    | <VariableDeclaration>
    | <Assignment>
    | <IfStatement>
    | <WhileStatement>
    | <Block>
    | "print" "(" <Expression> ")" ";"

<VariableDeclaration> ::= "var" <Identifier> "=" <Expression> ";"

<Assignment> ::= <Identifier> "=" <Expression> ";"

<IfStatement> ::=
    "if" "(" <Expression> ")" <Block>
    ("else" "if" "(" <Expression> ")" <Block>)*
    ("else" <Block>)?

<WhileStatement> ::= "while" "(" <Expression> ")" <Block>

<Block> ::= "{" <Statement>* "}"

<Expression> ::= <LogicalOrExpression>

<LogicalOrExpression> ::=
    <LogicalAndExpression>
    ( "||" <LogicalAndExpression> )*

<LogicalAndExpression> ::=
    <EqualityExpression>
    ( "&&" <EqualityExpression> )*

<EqualityExpression> ::=
    <RelationalExpression>
    ( ("==" | "!=") <RelationalExpression> )*

<RelationalExpression> ::=
    <AdditiveExpression>
    ( ("<" | ">" | "<=" | ">=") <AdditiveExpression> )*

<AdditiveExpression> ::=
    <MultiplicativeExpression>
    ( ("+" | "-") <MultiplicativeExpression> )*

<MultiplicativeExpression> ::=
    <UnaryExpression>
    ( ("*" | "/" | "%") <UnaryExpression> )*

<UnaryExpression> ::=
    ( "!" | "-" ) <UnaryExpression>
    | <PrimaryExpression>

<PrimaryExpression> ::=
    | <Number>
    | <String>
    | <Identifier>
    | "(" <Expression> ")"
    | "true"
    | "false"

<Number> ::= [0-9]+ ("." [0-9]+)?
<String> ::= "\"" [^"]* "\""
<Identifier> ::= [a-zA-Z_][a-zA-Z0-9_]*

```
