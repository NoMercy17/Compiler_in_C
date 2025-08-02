AtomC Compiler
A custom C compiler implementing the core phases of compilation: lexical analysis and parsing.
What it does

Lexer: Tokenizes C source code into meaningful symbols (keywords, identifiers, operators, numbers)
Parser: Builds Abstract Syntax Tree (AST) following C grammar rules and operator precedence

Key Components

lexer.c - Breaks source code into tokens, handles C syntax recognition
parser.c - Processes tokens into a parse tree, implements C grammar rules

Main Challenges Solved

Token Recognition: Pattern matching for C language elements (variables, keywords, operators)
Grammar Implementation: Handling operator precedence, associativity, and C syntax rules
AST Construction: Building proper tree structures for expressions and statements
