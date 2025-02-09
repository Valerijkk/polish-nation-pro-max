#include "graph.h"  // Подключаем заголовочный файл "graph.h", где объявлены все необходимые типы, структуры и прототипы

// Функция инициализации динамического массива токенов
void initTokenArray(TokenArray *arr) {
  arr->size = 0;                      // Устанавливаем текущее число элементов массива равным 0
  arr->capacity = 16;                 // Устанавливаем начальную емкость (capacity) равной 16
  arr->data = (Token *)malloc(sizeof(Token) * arr->capacity); // Выделяем динамическую память под массив токенов
}

// Функция добавления (push) нового токена в динамический массив
void pushTokenArray(TokenArray *arr, Token t) {
  if (arr->size == arr->capacity) {                                           // Проверяем, не заполнен ли массив
    arr->capacity *= 2;                                                       // Удваиваем емкость, если места нет
    arr->data = (Token *)realloc(arr->data, sizeof(Token) * arr->capacity);   // Перевыделяем память под новый размер
  }
  arr->data[arr->size++] = t; // Записываем новый токен в конец массива и увеличиваем size
}

// Функция освобождения памяти, которую занимал динамический массив токенов
void freeTokenArray(TokenArray *arr) {
  free(arr->data);       // Освобождаем память, на которую указывает arr->data
  arr->data = NULL;      // Ставим указатель в NULL, чтобы избежать висячих ссылок
  arr->size = 0;         // Обнуляем текущий размер массива
  arr->capacity = 0;     // Обнуляем емкость массива
}

// Функция инициализации стека токенов
void initTokenStack(TokenStack *st, int cap) {
  st->data = (Token *)malloc(sizeof(Token) * cap); // Выделяем память под массив токенов размером cap
  st->top = -1;                                    // Изначально в стеке нет элементов, ставим top = -1
  st->capacity = cap;                              // Запоминаем, какая вместимость у стека
}

// Функция добавления (push) нового токена на вершину стека
void pushTokenStack(TokenStack *st, Token t) {
  if (st->top < st->capacity - 1) { // Проверяем, не переполнен ли стек
    st->data[++st->top] = t;        // Сдвигаем вершину стека вперёд и записываем туда токен
  }
}

// Функция извлечения (pop) токена с вершины стека
Token popTokenStack(TokenStack *st) {
  Token tmp;                // Создаём временную структуру Token
  tmp.type = TOKEN_PLUS;    // Присваиваем начальное значение (тип не так важен, лишь бы был)
  tmp.value = 0.0;          // Начальное значение для value
  if (st->top >= 0) {       // Если стек не пуст
    tmp = st->data[st->top--]; // Забираем токен из стека и уменьшаем top
  }
  return tmp;               // Возвращаем извлечённый токен
}

// Функция для чтения (peek) верхнего токена стека без удаления
Token peekTokenStack(TokenStack *st) {
  Token tmp;               // Создаём временный токен
  tmp.type = TOKEN_PLUS;   // Начальное значение для типа
  tmp.value = 0.0;         // Начальное значение для числа
  if (st->top >= 0) {      // Если стек не пуст
    tmp = st->data[st->top]; // Присваиваем токен, лежащий на вершине
  }
  return tmp;              // Возвращаем токен (не удаляя его из стека)
}

// Функция проверки, пуст ли стек
int isStackEmpty(TokenStack *st) {
  return (st->top < 0);    // Если top < 0, значит элементов нет, стек пуст
}

// Функция освобождения памяти, которую занимал стек
void freeTokenStack(TokenStack *st) {
  free(st->data);          // Освобождаем память под массив токенов
  st->data = NULL;         // Обнуляем указатель, чтобы избежать висячих ссылок
  st->top = -1;            // Ставим top в -1 (стек теперь точно пуст)
  st->capacity = 0;        // Обнуляем емкость
}

// Функция определения приоритета операции (для алгоритма сортировочной станции)
int precedence(TokenType t) {
  if (t == TOKEN_PLUS || t == TOKEN_MINUS) return 1; // У + и - приоритет 1
  if (t == TOKEN_MULT || t == TOKEN_DIV)   return 2; // У * и / приоритет 2
  return 3; // У унарного минуса и функций приоритет выше (условно ставим 3)
}

// Функция проверки, является ли токен функцией (sin, cos, tan, ctg, sqrt, ln)
int isFunction(TokenType t) {
  return (t == TOKEN_SIN || t == TOKEN_COS || t == TOKEN_TAN ||
          t == TOKEN_CTG || t == TOKEN_SQRT || t == TOKEN_LN); // Возвращает 1 (true), если один из этих типов
}

// Функция проверки, является ли токен оператором (бинарный или унарный)
int isOperator(TokenType t) {
  return (t == TOKEN_PLUS || t == TOKEN_MINUS ||
          t == TOKEN_MULT || t == TOKEN_DIV || t == TOKEN_UMINUS); // Сюда включён и унарный минус
}

// Функция, упрощающая создание нового токена с заданным типом и значением
Token makeToken(TokenType type, double val) {
  Token tmp;     // Создаём временный токен
  tmp.type = type;  // Присваиваем ему тип
  tmp.value = val;  // Присваиваем ему числовое значение
  return tmp;    // Возвращаем созданную структуру
}

// Вспомогательная функция для быстрой вставки токена-функции (sin, cos и т.д.)
void addFunctionToken(TokenArray *arr, const char *str, int *i) {
  // Сравниваем подстроку (начиная с позиции *i) со строковыми литералами "sin", "cos" и т.д.
  if (!strncmp(&str[*i], "sin", 3)) {                     // Проверяем, не "sin" ли
    pushTokenArray(arr, makeToken(TOKEN_SIN, 0.0));        // Добавляем токен TOKEN_SIN
    (*i) += 3;                                            // Сдвигаем индекс i на 3
  } else if (!strncmp(&str[*i], "cos", 3)) {              // Проверяем, не "cos" ли
    pushTokenArray(arr, makeToken(TOKEN_COS, 0.0));        // Добавляем токен TOKEN_COS
    (*i) += 3;                                            // Сдвигаем индекс i на 3
  } else if (!strncmp(&str[*i], "tan", 3)) {              // Проверяем, не "tan" ли
    pushTokenArray(arr, makeToken(TOKEN_TAN, 0.0));        // Добавляем токен TOKEN_TAN
    (*i) += 3;                                            // Сдвигаем индекс i на 3
  } else if (!strncmp(&str[*i], "ctg", 3)) {              // Проверяем, не "ctg" ли
    pushTokenArray(arr, makeToken(TOKEN_CTG, 0.0));        // Добавляем токен TOKEN_CTG
    (*i) += 3;                                            // Сдвигаем индекс i на 3
  } else if (!strncmp(&str[*i], "sqrt", 4)) {             // Проверяем, не "sqrt" ли
    pushTokenArray(arr, makeToken(TOKEN_SQRT, 0.0));       // Добавляем токен TOKEN_SQRT
    (*i) += 4;                                            // Сдвигаем индекс i на 4
  } else if (!strncmp(&str[*i], "ln", 2)) {               // Проверяем, не "ln" ли
    pushTokenArray(arr, makeToken(TOKEN_LN, 0.0));         // Добавляем токен TOKEN_LN
    (*i) += 2;                                            // Сдвигаем индекс i на 2
  }
}

// Функция лексического разбора: преобразование входной строки в массив токенов
void tokenize(const char *str, TokenArray *arr) {
  int i = 0;                  // Текущий индекс для входной строки
  while (str[i] != '\0') {    // Пока не конец строки
    if (str[i] == ' ' || str[i] == '\t') {  // Пропускаем пробелы и табуляции
      i++;
    } else if ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
      // Если это цифра или точка, значит начинаются числа
      double val = 0.0;   // Переменная для результата
      char buf[64];       // Локальный буфер, куда будем собирать цифры
      int j = 0;          // Индекс в буфере buf

      // Цикл, пока символ является цифрой или точкой, копируем в buf
      while ((str[i] >= '0' && str[i] <= '9') || str[i] == '.') {
        buf[j++] = str[i++];
      }
      buf[j] = '\0';      // Завершаем строку нулевым байтом
      val = atof(buf);    // Преобразуем строку в число (double)
      pushTokenArray(arr, makeToken(TOKEN_NUMBER, val)); // Добавляем токен типа NUMBER с этим значением
    } else if (!strncmp(&str[i], "sin", 3) ||            // Проверяем, не начинается ли здесь
               !strncmp(&str[i], "cos", 3) ||            // какая-либо поддерживаемая функция
               !strncmp(&str[i], "tan", 3) ||
               !strncmp(&str[i], "ctg", 3) ||
               !strncmp(&str[i], "sqrt", 4) ||
               !strncmp(&str[i], "ln", 2)) {
      addFunctionToken(arr, str, &i); // Если да, пользуемся addFunctionToken, которая сама добавит соответствующий токен
    } else if (str[i] == 'x') {       // Если символ x
      pushTokenArray(arr, makeToken(TOKEN_X, 0.0)); // Добавляем токен X
      i++;
    } else if (str[i] == '+') {       // Если символ +
      pushTokenArray(arr, makeToken(TOKEN_PLUS, 0.0)); // Добавляем токен PLUS
      i++;
    } else if (str[i] == '-') {       // Если символ -
      // Определяем, унарный минус или бинарный
      // Унарным он будет, если i == 0 (первый символ строки)
      // или если предыдущий токен - оператор или левая скобка
      if (i == 0 || isOperator(arr->data[arr->size - 1].type) ||
          arr->data[arr->size - 1].type == TOKEN_LPAREN) {
        pushTokenArray(arr, makeToken(TOKEN_UMINUS, 0.0)); // Унарный минус
      } else {
        pushTokenArray(arr, makeToken(TOKEN_MINUS, 0.0));  // Бинарный минус
      }
      i++;
    } else if (str[i] == '*') {       // Если символ *
      pushTokenArray(arr, makeToken(TOKEN_MULT, 0.0)); // Добавляем токен MULT
      i++;
    } else if (str[i] == '/') {       // Если символ /
      pushTokenArray(arr, makeToken(TOKEN_DIV, 0.0));  // Добавляем токен DIV
      i++;
    } else if (str[i] == '(') {       // Если левая скобка
      pushTokenArray(arr, makeToken(TOKEN_LPAREN, 0.0)); // Добавляем токен LPAREN
      i++;
    } else if (str[i] == ')') {       // Если правая скобка
      pushTokenArray(arr, makeToken(TOKEN_RPAREN, 0.0)); // Добавляем токен RPAREN
      i++;
    } else {
      i++; // Если встретился неизвестный символ, просто пропускаем его
    }
  }
}

// Функция преобразования инфиксного выражения в постфиксное (Обратная Польская Нотация)
void toRPN(TokenArray *infix, TokenArray *postfix) {
  TokenStack stack;                          // Создаём стек, куда будем класть операторы, функции, скобки
  initTokenStack(&stack, infix->size + 10);  // Инициализируем стек с небольшим запасом

  // Проходим по всем токенам массива infix
  for (int i = 0; i < infix->size; i++) {
    Token t = infix->data[i];                // Текущий токен

    if (t.type == TOKEN_NUMBER || t.type == TOKEN_X) {  // Если это число или x
      pushTokenArray(postfix, t);                       // Сразу добавляем в выходную последовательность
    } else if (isFunction(t.type)) {                    // Если это функция (sin, cos и т.д.)
      pushTokenStack(&stack, t);                        // Кладём на стек
    } else if (t.type == TOKEN_UMINUS) {                // Если это унарный минус
      pushTokenStack(&stack, t);                        // Тоже кладём его на стек
    } else if (isOperator(t.type)) {                    // Если это бинарный оператор (+, -, *, /)
      // Пока стек не пуст, на вершине оператор, и его приоритет >= приоритета текущего, выгружаем его в postfix
      while (!isStackEmpty(&stack) &&
             isOperator(peekTokenStack(&stack).type) &&
             precedence(peekTokenStack(&stack).type) >= precedence(t.type)) {
        pushTokenArray(postfix, popTokenStack(&stack)); // Забираем верхний оператор со стека
      }
      pushTokenStack(&stack, t);                        // Кладём текущий оператор в стек
    } else if (t.type == TOKEN_LPAREN) {                // Если левая скобка
      pushTokenStack(&stack, t);                        // Кладём её на стек
    } else if (t.type == TOKEN_RPAREN) {                // Если правая скобка
      // Снимаем со стека, пока не найдём левую скобку
      while (!isStackEmpty(&stack) && peekTokenStack(&stack).type != TOKEN_LPAREN) {
        pushTokenArray(postfix, popTokenStack(&stack)); // Выгружаем оператор/функцию в выходной массив
      }
      if (!isStackEmpty(&stack) && peekTokenStack(&stack).type == TOKEN_LPAREN) {
        popTokenStack(&stack);                          // Убираем левую скобку со стека
      }
      // Если на вершине стека осталась функция, тоже перенесём её в postfix
      if (!isStackEmpty(&stack) && isFunction(peekTokenStack(&stack).type)) {
        pushTokenArray(postfix, popTokenStack(&stack));
      }
    }
  }

  // По завершении прохода по infix снимаем со стека все оставшиеся операторы
  while (!isStackEmpty(&stack)) {
    pushTokenArray(postfix, popTokenStack(&stack));
  }

  // Освобождаем стек
  freeTokenStack(&stack);
}

// Функция вычисления математической функции (sin, cos, tan и т.д.)
double computeFunction(TokenType t, double val) {
  if (t == TOKEN_SIN)  return sin(val);       // sin
  if (t == TOKEN_COS)  return cos(val);       // cos
  if (t == TOKEN_TAN)  return tan(val);       // tan
  if (t == TOKEN_CTG)  return 1.0 / tan(val); // ctg = 1/tan
  if (t == TOKEN_SQRT) return sqrt(val);      // sqrt
  if (t == TOKEN_LN)   return log(val);       // ln
  return 0.0;                                 // Если ничего не подошло, возвращаем 0.0
}

// Функция вычисления результата выражения в постфиксной форме при заданном x
double evalRPN(TokenArray *postfix, double xval) {
  double stack[256];  // Локальный массив, который будем использовать как стек
  int top = -1;       // Изначально стек пуст (top = -1)

  // Проходим по всем токенам в массиве postfix
  for (int i = 0; i < postfix->size; i++) {
    Token t = postfix->data[i]; // Текущий токен

    if (t.type == TOKEN_NUMBER) {    // Если это число
      stack[++top] = t.value;        // Кладём его значение на вершину стека
    } else if (t.type == TOKEN_X) {  // Если это переменная x
      stack[++top] = xval;           // Кладём в стек значение xval
    } else if (t.type == TOKEN_UMINUS) { // Если это унарный минус
      stack[top] = -stack[top];      // Меняем знак верхнего элемента стека
    } else if (isOperator(t.type)) { // Если это бинарный оператор (+, -, *, /)
      double b = stack[top--];       // Снимаем верхний операнд со стека (правый)
      double a = stack[top--];       // Снимаем следующий операнд (левый)
      if (t.type == TOKEN_PLUS)       stack[++top] = a + b;  // Выполняем сложение
      else if (t.type == TOKEN_MINUS) stack[++top] = a - b;  // Вычитание
      else if (t.type == TOKEN_MULT)  stack[++top] = a * b;  // Умножение
      else if (t.type == TOKEN_DIV)   stack[++top] = a / b;  // Деление
    } else if (isFunction(t.type)) { // Если это функция (sin, cos, tan и т.д.)
      stack[top] = computeFunction(t.type, stack[top]); // Применяем функцию к верхушке стека
    }
  }

  return stack[top]; // По итогам вычислений в стеке остаётся одно значение — результат
}

// Главная функция программы
int main(void) {
  char input[256];                           // Буфер для входной строки

  if (!fgets(input, sizeof(input), stdin)) { // Пытаемся считать строку, если неудачно (EOF), завершаем
    return 0;                                // Возвращаем 0 — нормальное завершение
  }

  size_t len = strlen(input);                // Определяем длину полученной строки
  if (len > 0 && input[len - 1] == '\n') {   // Если последний символ — перевод строки
    input[len - 1] = '\0';                  // Убираем его, заменяя на терминальный ноль
  }

  TokenArray infix, postfix;                // Создаем две структуры для инфиксной и постфиксной записи
  initTokenArray(&infix);                   // Инициализируем динамический массив infix
  initTokenArray(&postfix);                 // Инициализируем динамический массив postfix

  tokenize(input, &infix);                  // Разбиваем входную строку на токены, заполняя infix
  toRPN(&infix, &postfix);                  // Преобразуем инфиксное выражение infix в постфиксное (postfix)

  char canvas[25][80];                      // Создаём "полотно" 25 строк на 80 столбцов
  for (int r = 0; r < 25; r++) {            // Проходим по всем строкам (r)
    for (int c = 0; c < 80; c++) {          // Проходим по всем столбцам (c)
      canvas[r][c] = '.';                   // Заполняем всё точками ('.')
    }
  }

  // Заполняем "звёздочками" ту часть полотна, где располагается график
  // Проходим по горизонтали от c=0 до c=79
  for (int c = 0; c < 80; c++) {
    double x = 4.0 * M_PI * c / 79.0;       // Сопоставляем c с x в интервале [0..4*pi]
    double yVal = evalRPN(&postfix, x);     // Вычисляем значение функции при данном x
    if (yVal >= -1.0 && yVal <= 1.0) {      // Если результат попал в диапазон от -1 до 1
      double scaled = 12.0 + yVal * 12.0;   // Линеарно переводим y из [-1..1] в [0..24]
      int row = (int)round(scaled);         // Округляем результат, чтобы получить индекс строки
      if (row >= 0 && row < 25) {           // Проверяем, не вышли ли за границы полотна
        canvas[row][c] = '*';               // Ставим звёздочку там, где график
      }
    }
  }

  // Выводим итоговое "полотно" на экран
  for (int r = 0; r < 25; r++) {            // Идём по строкам
    for (int c = 0; c < 80; c++) {          // Идём по столбцам
      putchar(canvas[r][c]);               // Печатаем символ
    }
    putchar('\n');                          // Переходим на новую строку
  }

  // Освобождаем динамические массивы токенов
  freeTokenArray(&infix);
  freeTokenArray(&postfix);

  return 0;                                 // Завершаем программу с кодом 0
}
