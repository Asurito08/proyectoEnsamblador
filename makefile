# Makefile corregido para estructura actual

CC = gcc
SRC = src/main.c \
      src/menu.c \
      src/opciones.c \
      src/stackManager.c

BUILD_DIR = build
TARGET = $(BUILD_DIR)/mi_app

CFLAGS = $(shell pkg-config --cflags gtk4)
LDFLAGS = $(shell pkg-config --libs gtk4)

# Compilar
all: $(TARGET)

$(TARGET): $(SRC)
	mkdir -p $(BUILD_DIR)
	$(CC) $(CFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

# Ejecutar
run: $(TARGET)
	./$(TARGET)

# Limpiar
clean:
	rm -rf $(BUILD_DIR)
