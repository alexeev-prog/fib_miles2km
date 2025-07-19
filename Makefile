# Определение директорий
SRC_DIR := src
BIN_DIR := bin
TARGET := $(BIN_DIR)/fib_miles2km

# Поиск всех исходных файлов .c
SRC_FILES := $(wildcard $(SRC_DIR)/*.c)
# Генерация списка объектных файлов
OBJ_FILES := $(patsubst $(SRC_DIR)/%.c,$(BIN_DIR)/%.o,$(SRC_FILES))

# Компилятор и флаги
CC := gcc
CFLAGS := -Wall -Wextra
LDFLAGS :=

.PHONY: all clean

all: $(BIN_DIR) $(TARGET)

# Создание директории bin при необходимости
$(BIN_DIR):
	@mkdir -p $@

# Компоновка исполняемого файла
$(TARGET): $(OBJ_FILES)
	$(CC) $(LDFLAGS) $^ -o $@

# Компиляция исходных файлов
$(BIN_DIR)/%.o: $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c $< -o $@

# Очистка артефактов сборки
clean:
	@rm -rf $(BIN_DIR)
