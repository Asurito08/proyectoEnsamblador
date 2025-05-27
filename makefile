CC = gcc
NASM = nasm

SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

C_SRC = $(filter-out $(SRC_DIR)/termctl.c, $(wildcard $(SRC_DIR)/*.c))
C_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRC))

ASM_SRC = $(SRC_DIR)/matrizElementos.asm
ASM_OBJ = $(BUILD_DIR)/matrizElementos.o
ASM_BIN = $(BIN_DIR)/matrizElementos

WRAPPER_SRC = $(SRC_DIR)/termctl.c
WRAPPER_BIN = $(BIN_DIR)/termctl

TARGET = $(BIN_DIR)/mi_app

CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Solo compila la app con GTK por defecto
all: $(TARGET)

# Compilar ejecutable principal con GTK
$(TARGET): $(C_OBJ)
	mkdir -p $(BIN_DIR)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilar archivos .c (GTK)
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Ejecutar programa principal (GTK)
run: $(TARGET)
	./$(TARGET)

# Limpiar todo
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)
