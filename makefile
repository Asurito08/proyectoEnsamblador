# Compiladores
CC = gcc
NASM = nasm

# Directorios
SRC_DIR = src
BUILD_DIR = build
BIN_DIR = bin

# Archivos fuente C
C_SRC = $(filter-out $(SRC_DIR)/termctl.c, $(wildcard $(SRC_DIR)/*.c))
C_OBJ = $(patsubst $(SRC_DIR)/%.c, $(BUILD_DIR)/%.o, $(C_SRC))

# Archivo ensamblador
ASM_SRC = $(SRC_DIR)/matrizElementos.asm
ASM_OBJ = $(BUILD_DIR)/matrizElementos.o

# Nombre del ejecutable
TARGET = $(BIN_DIR)/mi_app

# Flags de compilación y enlace
CFLAGS = -g -O0 $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Regla por defecto
all: $(TARGET)

# Crear ejecutable enlazando C + ASM
$(TARGET): $(C_OBJ) $(ASM_OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) -no-pie $(CFLAGS) -o $@ $^ $(LDFLAGS)

# Compilar archivos C a objetos
$(BUILD_DIR)/%.o: $(SRC_DIR)/%.c
	@mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -c $< -o $@

# Compilar archivo ASM a objeto
$(ASM_OBJ): $(ASM_SRC)
	@mkdir -p $(BUILD_DIR)
	$(NASM) -f elf64 $< -o $@

# Ejecutar la aplicación normalmente
run: $(TARGET)
	ulimit -c unlimited && ./$(TARGET)

# Ejecutar con Valgrind
debug: $(TARGET)
	ulimit -c unlimited && valgrind --leak-check=full --show-leak-kinds=all ./$(TARGET)

# Limpiar archivos generados
clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR) core* *.core
