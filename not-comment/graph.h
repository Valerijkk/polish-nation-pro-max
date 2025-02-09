#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef enum {
  TOKEN_NUMBER,
  TOKEN_X,
  TOKEN_PLUS,
  TOKEN_MINUS,
  TOKEN_MULT,
  TOKEN_DIV,
  TOKEN_LPAREN,
  TOKEN_RPAREN,
  TOKEN_SIN,
  TOKEN_COS,
  TOKEN_TAN,
  TOKEN_CTG,
  TOKEN_SQRT,
  TOKEN_LN,
  TOKEN_UMINUS
} TokenType;

typedef struct {
  TokenType type;
  double value;
} Token;

typedef struct {
  Token *data;
  int size;
  int capacity;
} TokenArray;

typedef struct {
  Token *data;
  int top;
  int capacity;
} TokenStack;

void initTokenArray(TokenArray *arr);
void pushTokenArray(TokenArray *arr, Token t);
void freeTokenArray(TokenArray *arr);

void initTokenStack(TokenStack *st, int cap);
void pushTokenStack(TokenStack *st, Token t);
Token popTokenStack(TokenStack *st);
Token peekTokenStack(const TokenStack *st);
int isStackEmpty(const TokenStack *st);
void freeTokenStack(TokenStack *st);

int precedence(TokenType t);
int isFunction(TokenType t);
int isOperator(TokenType t);
Token makeToken(TokenType type, double val);

void tokenize(const char *str, TokenArray *arr);
void toRPN(const TokenArray *infix, TokenArray *postfix);
double computeFunction(TokenType t, double val);
double evalRPN(const TokenArray *postfix, double xval);
void fillCanvas(char canvas[25][80], const TokenArray *postfix);
void printCanvas(char canvas[25][80]);

#endif
