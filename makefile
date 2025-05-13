CC = gcc

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

SRC = $(wildcard $(SRC_DIR)/*.c)
OBJ = $(patsubst $(SRC_DIR)/%.c,$(BUILD_DIR)/%.o,$(SRC))

TARGET = $(BIN_DIR)/mi_app

CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Compilar
all: $(TARGET)

# Regla para crear el ejecutable
$(TARGET): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Regla para compilar cada .c en .o
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar
run: $(TARGET)
	./$(TARGET)

# Limpiar
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)


# Compilar y correr el archivo nasm para pruebas
nasm:
	mkdir -p $(BIN_DIR)
	mkdir -p $(BUILD_DIR)
	nasm -f elf64 $(SRC_DIR)/matrizElementos.asm -o $(BUILD_DIR)/matrizElementos.o
	ld $(BUILD_DIR)/matrizElementos.o -o $(BIN_DIR)/matrizElementos

runasm:
	./$(BIN_DIR)/matrizElementos