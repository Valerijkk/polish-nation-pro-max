#ifndef GRAPH_H                           /* Защита от повторного включения */
#define GRAPH_H

#include <stdio.h>                       /* Подключаем для ввода-вывода */
#include <stdlib.h>                      /* malloc, free, atof и т.д. */
#include <string.h>                      /* Работа со строками: strncmp, strlen */
#include <math.h>                        /* Математические функции sin, cos и т.д. */

/*-----------------------------------------------------------------------------
 * Перечисление типов токенов для математического выражения
 *-----------------------------------------------------------------------------*/
typedef enum {
  TOKEN_NUMBER,   /* Числовое значение */
  TOKEN_X,        /* Переменная x */
  TOKEN_PLUS,     /* Оператор + */
  TOKEN_MINUS,    /* Оператор - */
  TOKEN_MULT,     /* Оператор * */
  TOKEN_DIV,      /* Оператор / */
  TOKEN_LPAREN,   /* Левая скобка "(" */
  TOKEN_RPAREN,   /* Правая скобка ")" */
  TOKEN_SIN,      /* Функция sin */
  TOKEN_COS,      /* Функция cos */
  TOKEN_TAN,      /* Функция tan */
  TOKEN_CTG,      /* Функция ctg (1/tan) */
  TOKEN_SQRT,     /* Функция sqrt */
  TOKEN_LN,       /* Функция ln */
  TOKEN_UMINUS    /* Унарный минус */
} TokenType;

/*-----------------------------------------------------------------------------
 * Структура, описывающая один токен (тип + значение)
 *-----------------------------------------------------------------------------*/
typedef struct {
  TokenType type; /* Тип токена из перечисления выше */
  double value;   /* Числовое значение (актуально, если type == TOKEN_NUMBER) */
} Token;

/*-----------------------------------------------------------------------------
 * Динамический массив токенов
 *-----------------------------------------------------------------------------*/
typedef struct {
  Token *data;    /* Указатель на блок памяти под токены */
  int size;       /* Текущее количество токенов */
  int capacity;   /* Текущая емкость (сколько токенов умещается) */
} TokenArray;

/*-----------------------------------------------------------------------------
 * Стек для токенов (для алгоритма перевода в ОПН)
 *-----------------------------------------------------------------------------*/
typedef struct {
  Token *data;    /* Указатель на массив (стек) */
  int top;        /* Индекс вершины стека (>= 0) */
  int capacity;   /* Максимальная вместимость стека */
} TokenStack;

/*-----------------------------------------------------------------------------
 * Прототипы всех функций
 *-----------------------------------------------------------------------------*/

/* Инициализация и работа с динамическим массивом токенов */
void initTokenArray(TokenArray *arr);
void pushTokenArray(TokenArray *arr, Token t);
void freeTokenArray(TokenArray *arr);

/* Инициализация и работа со стеком токенов */
void initTokenStack(TokenStack *st, int cap);
void pushTokenStack(TokenStack *st, Token t);
Token popTokenStack(TokenStack *st);
Token peekTokenStack(const TokenStack *st);
int isStackEmpty(const TokenStack *st);
void freeTokenStack(TokenStack *st);

/* Проверки и создание токенов */
int precedence(TokenType t);
int isFunction(TokenType t);
int isOperator(TokenType t);
Token makeToken(TokenType type, double val);

/* Лексический разбор (строка -> токены) */
void tokenize(const char *str, TokenArray *arr);

/* Преобразование инфиксной записи в ОПН */
void toRPN(const TokenArray *infix, TokenArray *postfix);

/* Вычисление математической функции типа sin/cos/... */
double computeFunction(TokenType t, double val);

/* Вычисление выражения в ОПН при заданном x */
double evalRPN(const TokenArray *postfix, double xval);

/* Заполнение холста (25x80) звёздочками по значению функции */
void fillCanvas(char canvas[25][80], const TokenArray *postfix);

/* Печать холста (25x80) на экран */
void printCanvas(char canvas[25][80]);

#endif /* GRAPH_H */
