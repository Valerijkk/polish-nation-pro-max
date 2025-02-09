#ifndef GRAPH_H                // Если не определён идентификатор GRAPH_H
#define GRAPH_H                // Определяем идентификатор GRAPH_H, чтобы избежать повторных включений

#include <stdlib.h>            // Стандартная библиотека для функций malloc, free, atof и т.д.
#include <string.h>            // Библиотека для работы со строками (strncmp, strlen и т.д.)
#include <math.h>              // Математические функции (sin, cos, tan, sqrt, log и т.д.)

// Перечисление типов токенов
typedef enum {
  TOKEN_NUMBER,  // Числовое значение
  TOKEN_X,       // Переменная x
  TOKEN_PLUS,    // Операция +
  TOKEN_MINUS,   // Операция -
  TOKEN_MULT,    // Операция *
  TOKEN_DIV,     // Операция /
  TOKEN_LPAREN,  // Левая скобка (
  TOKEN_RPAREN,  // Правая скобка )
  TOKEN_SIN,     // Функция sin
  TOKEN_COS,     // Функция cos
  TOKEN_TAN,     // Функция tan
  TOKEN_CTG,     // Функция ctg = 1/tan
  TOKEN_SQRT,    // Функция sqrt
  TOKEN_LN,      // Функция ln
  TOKEN_UMINUS   // Унарный минус
} TokenType;

// Структура для хранения одного токена
typedef struct {
  TokenType type; // Тип токена
  double value;   // Числовое значение (актуально для числа)
} Token;

// Динамический массив токенов
typedef struct {
  Token *data;    // Указатель на массив токенов
  int size;       // Текущее количество токенов
  int capacity;   // Текущая емкость массива
} TokenArray;

// Стек для токенов (используется в алгоритме сортировочной станции)
typedef struct {
  Token *data;    // Указатель на массив (стек)
  int top;        // Индекс вершины стека
  int capacity;   // Максимальный размер стека
} TokenStack;

// Прототипы функций для работы с динамическим массивом
void initTokenArray(TokenArray *arr);
void pushTokenArray(TokenArray *arr, Token t);
void freeTokenArray(TokenArray *arr);

// Прототипы функций для работы со стеком токенов
void initTokenStack(TokenStack *st, int cap);
void pushTokenStack(TokenStack *st, Token t);
Token popTokenStack(TokenStack *st);
Token peekTokenStack(TokenStack *st);
int isStackEmpty(TokenStack *st);
void freeTokenStack(TokenStack *st);

// Прототипы служебных функций
int precedence(TokenType t);
int isFunction(TokenType t);
int isOperator(TokenType t);
Token makeToken(TokenType type, double val);
void addFunctionToken(TokenArray *arr, const char *str, int *i);

// Прототип функции для лексического разбора строки
void tokenize(const char *str, TokenArray *arr);

// Прототип функции для преобразования в обратную польскую нотацию
void toRPN(TokenArray *infix, TokenArray *postfix);

// Прототип функции вычисления математической функции
double computeFunction(TokenType t, double val);

// Прототип функции вычисления выражения в ОПН
double evalRPN(TokenArray *postfix, double xval);

#endif // GRAPH_H
