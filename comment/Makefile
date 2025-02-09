# Makefile (с комментариями)

# Название исполняемого файла
TARGET = graph

# Папка, куда складываем объектные и бинарные файлы
BUILD_DIR = build

# Папка с исходниками
SRC_DIR = src

# Компилятор
CC = gcc

# Флаги компилятора:
# -Wall      - показывать все предупреждения
# -Werror    - считать предупреждения за ошибки
# -std=c11   - использовать стандарт C11
CFLAGS = -Wall -Wextra -Werror -std=c11

# Цель, которая собирает всё (по умолчанию)
all: $(BUILD_DIR)/$(TARGET)

# Правило сборки исполняемого файла из .c и .h
# Подтягиваем зависимость: при изменении graph.h или graph.c - пересборка
$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/graph.c $(SRC_DIR)/graph.h
	mkdir -p $(BUILD_DIR) \
	&& $(CC) $(CFLAGS) $(SRC_DIR)/graph.c -o $(BUILD_DIR)/$(TARGET) -lm

# Правило очистки: удаляем бинарник
clean:
	rm -f $(BUILD_DIR)/$(TARGET)
