// Elijah Rosal - CS4080 - Homework 11, Chapter 22 Question 1
// 4.23.2026
/*
Code below has been modified for Question 1 for Chapter 22 of Crafting Interpreters.

Replaces linear local-variable resolution with a hash-bucket based lookup
structure to reduce average identifier lookup cost while preserving scope
and shadowing behavior.
*/

#ifndef clox_scanner_h
#define clox_scanner_h
typedef enum {
    // Single-character tokens.
    TOKEN_LEFT_PAREN, TOKEN_RIGHT_PAREN,
    TOKEN_LEFT_BRACE, TOKEN_RIGHT_BRACE,
    TOKEN_COMMA, TOKEN_DOT, TOKEN_MINUS, TOKEN_PLUS,
    TOKEN_COLON,
    TOKEN_SEMICOLON, TOKEN_SLASH, TOKEN_STAR,
    // One or two character tokens.
    TOKEN_BANG, TOKEN_BANG_EQUAL,
    TOKEN_EQUAL, TOKEN_EQUAL_EQUAL,
    TOKEN_GREATER, TOKEN_GREATER_EQUAL,
    TOKEN_LESS, TOKEN_LESS_EQUAL,
    // Literals.
    TOKEN_IDENTIFIER, TOKEN_STRING, TOKEN_NUMBER,
    // Keywords.
    TOKEN_AND, TOKEN_CASE, TOKEN_CLASS, TOKEN_DEFAULT,
    TOKEN_ELSE, TOKEN_FALSE,
    TOKEN_FOR, TOKEN_FUN, TOKEN_IF, TOKEN_NIL, TOKEN_OR,
    TOKEN_PRINT, TOKEN_RETURN, TOKEN_SUPER, TOKEN_THIS,
    TOKEN_SWITCH, TOKEN_TRUE, TOKEN_VAR, TOKEN_WHILE,
    TOKEN_ERROR, TOKEN_EOF
   } TokenType;
typedef struct {
    TokenType type;
    const char* start;
    int length;
    int line;
} Token;
void initScanner(const char* source);
Token scanToken();
#endif