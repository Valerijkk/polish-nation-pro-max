TARGET = graph
BUILD_DIR = build
SRC_DIR = src
CC = gcc
CFLAGS = -Wall -Wextra -Werror -std=c11

all: $(BUILD_DIR)/$(TARGET)

$(BUILD_DIR)/$(TARGET): $(SRC_DIR)/graph.c $(SRC_DIR)/graph.h
	mkdir -p $(BUILD_DIR) \
	&& $(CC) $(CFLAGS) $(SRC_DIR)/graph.c -o $(BUILD_DIR)/$(TARGET) -lm

clean:
	rm -f $(BUILD_DIR)/$(TARGET)
