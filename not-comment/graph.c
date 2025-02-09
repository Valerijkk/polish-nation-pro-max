#include "graph.h"

void initTokenArray(TokenArray *arr) {
  arr->size = 0;
  arr->capacity = 16;
  arr->data = (Token *)malloc(sizeof(Token) * arr->capacity);
}

void pushTokenArray(TokenArray *arr, Token t) {
  if (arr->size == arr->capacity) {
    arr->capacity *= 2;
    arr->data = (Token *)realloc(arr->data, sizeof(Token) * arr->capacity);
  }
  arr->data[arr->size] = t;
  arr->size++;
}

void freeTokenArray(TokenArray *arr) {
  free(arr->data);
  arr->data = NULL;
  arr->size = 0;
  arr->capacity = 0;
}

void initTokenStack(TokenStack *st, int cap) {
  st->data = (Token *)malloc(sizeof(Token) * cap);
  st->top = -1;
  st->capacity = cap;
}

void pushTokenStack(TokenStack *st, Token t) {
  if (st->top < st->capacity - 1) {
    st->top++;
    st->data[st->top] = t;
  }
}

Token popTokenStack(TokenStack *st) {
  Token tmp;
  tmp.type = TOKEN_PLUS;
  tmp.value = 0.0;
  if (st->top >= 0) {
    tmp = st->data[st->top];
    st->top--;
  }
  return tmp;
}

Token peekTokenStack(const TokenStack *st) {
  Token tmp;
  tmp.type = TOKEN_PLUS;
  tmp.value = 0.0;
  if (st->top >= 0) {
    tmp = st->data[st->top];
  }
  return tmp;
}

int isStackEmpty(const TokenStack *st) {
  return (st->top < 0);
}

void freeTokenStack(TokenStack *st) {
  free(st->data);
  st->data = NULL;
  st->top = -1;
  st->capacity = 0;
}

int precedence(TokenType t) {
  int res = 3;
  if (t == TOKEN_PLUS || t == TOKEN_MINUS) {
    res = 1;
  } else if (t == TOKEN_MULT || t == TOKEN_DIV) {
    res = 2;
  }
  return res;
}

int isFunction(TokenType t) {
  int r = 0;
  if (t == TOKEN_SIN || t == TOKEN_COS ||
      t == TOKEN_TAN || t == TOKEN_CTG ||
      t == TOKEN_SQRT || t == TOKEN_LN) {
    r = 1;
  }
  return r;
}

int isOperator(TokenType t) {
  int r = 0;
  if (t == TOKEN_PLUS || t == TOKEN_MINUS ||
      t == TOKEN_MULT || t == TOKEN_DIV ||
      t == TOKEN_UMINUS) {
    r = 1;
  }
  return r;
}

Token makeToken(TokenType type, double val) {
  Token tmp;
  tmp.type = type;
  tmp.value = val;
  return tmp;
}

static void readNumber(const char *str, int *i, TokenArray *arr) {
  char buf[64];
  int j = 0;
  while ((str[*i] >= '0' && str[*i] <= '9') || str[*i] == '.') {
    buf[j] = str[*i];
    j++;
    (*i)++;
  }
  buf[j] = '\0';
  pushTokenArray(arr, makeToken(TOKEN_NUMBER, atof(buf)));
}

static void addFunctionToken(const char *str, int *i, TokenArray *arr) {
  if (!strncmp(&str[*i], "sin", 3)) {
    pushTokenArray(arr, makeToken(TOKEN_SIN, 0.0));
    (*i) += 3;
  } else if (!strncmp(&str[*i], "cos", 3)) {
    pushTokenArray(arr, makeToken(TOKEN_COS, 0.0));
    (*i) += 3;
  } else if (!strncmp(&str[*i], "tan", 3)) {
    pushTokenArray(arr, makeToken(TOKEN_TAN, 0.0));
    (*i) += 3;
  } else if (!strncmp(&str[*i], "ctg", 3)) {
    pushTokenArray(arr, makeToken(TOKEN_CTG, 0.0));
    (*i) += 3;
  } else if (!strncmp(&str[*i], "sqrt", 4)) {
    pushTokenArray(arr, makeToken(TOKEN_SQRT, 0.0));
    (*i) += 4;
  } else if (!strncmp(&str[*i], "ln", 2)) {
    pushTokenArray(arr, makeToken(TOKEN_LN, 0.0));
    (*i) += 2;
  }
}

void tokenize(const char *str, TokenArray *arr) {
  int i = 0;
  while (str[i] != '\0') {
    if (str[i] == ' ' || str[i] == '\t') {
      i++;
    } else if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
      readNumber(str, &i, arr);
    } else if (!strncmp(&str[i], "sin", 3) ||
               !strncmp(&str[i], "cos", 3) ||
               !strncmp(&str[i], "tan", 3) ||
               !strncmp(&str[i], "ctg", 3) ||
               !strncmp(&str[i], "sqrt", 4) ||
               !strncmp(&str[i], "ln", 2)) {
      addFunctionToken(str, &i, arr);
    } else if (str[i] == 'x') {
      pushTokenArray(arr, makeToken(TOKEN_X, 0.0));
      i++;
    } else if (str[i] == '+') {
      pushTokenArray(arr, makeToken(TOKEN_PLUS, 0.0));
      i++;
    } else if (str[i] == '-') {
      if (i == 0 || isOperator(arr->data[arr->size - 1].type) ||
          arr->data[arr->size - 1].type == TOKEN_LPAREN) {
        pushTokenArray(arr, makeToken(TOKEN_UMINUS, 0.0));
      } else {
        pushTokenArray(arr, makeToken(TOKEN_MINUS, 0.0));
      }
      i++;
    } else if (str[i] == '*') {
      pushTokenArray(arr, makeToken(TOKEN_MULT, 0.0));
      i++;
    } else if (str[i] == '/') {
      pushTokenArray(arr, makeToken(TOKEN_DIV, 0.0));
      i++;
    } else if (str[i] == '(') {
      pushTokenArray(arr, makeToken(TOKEN_LPAREN, 0.0));
      i++;
    } else if (str[i] == ')') {
      pushTokenArray(arr, makeToken(TOKEN_RPAREN, 0.0));
      i++;
    } else {
      i++;
    }
  }
}

void toRPN(const TokenArray *infix, TokenArray *postfix) {
  TokenStack stack;
  initTokenStack(&stack, infix->size + 10);
  for (int i = 0; i < infix->size; i++) {
    Token t = infix->data[i];
    if (t.type == TOKEN_NUMBER || t.type == TOKEN_X) {
      pushTokenArray(postfix, t);
    } else if (isFunction(t.type) || t.type == TOKEN_UMINUS) {
      pushTokenStack(&stack, t);
    } else if (isOperator(t.type)) {
      while (!isStackEmpty(&stack) &&
             isOperator(peekTokenStack(&stack).type) &&
             precedence(peekTokenStack(&stack).type) >= precedence(t.type)) {
        pushTokenArray(postfix, popTokenStack(&stack));
      }
      pushTokenStack(&stack, t);
    } else if (t.type == TOKEN_LPAREN) {
      pushTokenStack(&stack, t);
    } else if (t.type == TOKEN_RPAREN) {
      while (!isStackEmpty(&stack) &&
             peekTokenStack(&stack).type != TOKEN_LPAREN) {
        pushTokenArray(postfix, popTokenStack(&stack));
      }
      if (!isStackEmpty(&stack) &&
          peekTokenStack(&stack).type == TOKEN_LPAREN) {
        popTokenStack(&stack);
      }
      if (!isStackEmpty(&stack) && isFunction(peekTokenStack(&stack).type)) {
        pushTokenArray(postfix, popTokenStack(&stack));
      }
    }
  }
  while (!isStackEmpty(&stack)) {
    pushTokenArray(postfix, popTokenStack(&stack));
  }
  freeTokenStack(&stack);
}

double computeFunction(TokenType t, double val) {
  double r = 0.0;
  if (t == TOKEN_SIN) r = sin(val);
  else if (t == TOKEN_COS) r = cos(val);
  else if (t == TOKEN_TAN) r = tan(val);
  else if (t == TOKEN_CTG) r = 1.0 / tan(val);
  else if (t == TOKEN_SQRT) r = sqrt(val);
  else if (t == TOKEN_LN) r = log(val);
  return r;
}

double evalRPN(const TokenArray *postfix, double xval) {
  double stack[256];
  int top = -1;
  int count = postfix->size;
  for (int i = 0; i < count; i++) {
    Token t = postfix->data[i];
    if (t.type == TOKEN_NUMBER) {
      top++;
      stack[top] = t.value;
    } else if (t.type == TOKEN_X) {
      top++;
      stack[top] = xval;
    } else if (t.type == TOKEN_UMINUS) {
      stack[top] = -stack[top];
    } else if (isOperator(t.type) && t.type != TOKEN_UMINUS) {
      double b = stack[top];
      top--;
      double a = stack[top];
      top--;
      if (t.type == TOKEN_PLUS) stack[++top] = a + b;
      else if (t.type == TOKEN_MINUS) stack[++top] = a - b;
      else if (t.type == TOKEN_MULT) stack[++top] = a * b;
      else stack[++top] = a / b;
    } else if (isFunction(t.type)) {
      stack[top] = computeFunction(t.type, stack[top]);
    }
  }
  return stack[top];
}

void fillCanvas(char canvas[25][80], const TokenArray *postfix) {
  for (int r = 0; r < 25; r++) {
    for (int c = 0; c < 80; c++) {
      canvas[r][c] = '.';
    }
  }
  for (int c = 0; c < 80; c++) {
    double x = 4.0 * M_PI * (double)c / 79.0;
    double yVal = evalRPN(postfix, x);
    if (yVal >= -1.0 && yVal <= 1.0) {
      double scaled = 12.0 + yVal * 12.0;
      int row = (int)round(scaled);
      if (row >= 0 && row < 25) {
        canvas[row][c] = '*';
      }
    }
  }
}

void printCanvas(char canvas[25][80]) {
  for (int r = 0; r < 25; r++) {
    for (int c = 0; c < 80; c++) {
      putchar(canvas[r][c]);
    }
    putchar('\n');
  }
}

int main(void) {
  int retVal = 0;
  char input[256];
  if (!fgets(input, sizeof(input), stdin)) {
    retVal = 0;
  } else {
    size_t len = strlen(input);
    if (len > 0 && input[len - 1] == '\n') {
      input[len - 1] = '\0';
    }
    TokenArray infix;
    TokenArray postfix;
    initTokenArray(&infix);
    initTokenArray(&postfix);
    tokenize(input, &infix);
    toRPN(&infix, &postfix);
    char canvas[25][80];
    fillCanvas(canvas, &postfix);
    printCanvas(canvas);
    freeTokenArray(&infix);
    freeTokenArray(&postfix);
    retVal = 0;
  }
  return retVal;
}
